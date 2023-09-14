/*
 * Copyright (c) 2015, Swiss Federal Institute of Technology (ETH Zurich).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author:  Federico Ferrari
 *          Reto Da Forno
 *          Marco Zimmerling
 */

/*  Modified by: Sanjay Charran <sanjaycharran@gmail.com>   */

#include "ns3/glossy.h"

#include "ns3/log.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace lora_mesh {
 
NS_LOG_COMPONENT_DEFINE("Glossy");
    
TypeId 
Glossy::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::Glossy")
        .SetParent<Object>()
        .SetGroupName("lora_mesh");
        
    return tid;
}
    
Glossy::Glossy()
{    
    m_timeout_delay_seconds = 1;
    m_glossy_state.payload = (uint8_t *)calloc(RF_CONF_MAX_PKT_LEN, sizeof(uint8_t));
}
 
Glossy::~Glossy()
{
    Stop();
    free(m_glossy_state.payload);
}

void
Glossy::Start(uint16_t initiator_id, Ptr<Packet> packet, uint8_t n_tx_max,
              glossy_sync_t sync, glossy_rf_cal_t rf_cal)
{
    NS_LOG_FUNCTION (this << initiator_id << packet << n_tx_max << sync << rf_cal);
    
    GlossyHeader new_header;
    
    /* reset the data structure */
    m_glossy_state.active = 1;
    packet->CopyData(m_glossy_state.payload, packet->GetSize());
    m_glossy_state.payload_len = packet->GetSize() - new_header.GetSerializedSize();
    m_glossy_state.n_rx = 0;
    m_glossy_state.n_tx = 0;
    m_glossy_state.relay_cnt_last_rx = 0;
    m_glossy_state.relay_cnt_last_tx = 0;
    m_glossy_state.t_ref_updated = 0;
    m_glossy_state.T_slot_sum = 0;
    m_glossy_state.n_T_slot = 0;
    
    /* prepare the Glossy header, with the information known so far */
    m_glossy_state.header.SetInitiatorId(initiator_id);
    m_glossy_state.header.SetPktType(GLOSSY_COMMON_HEADER | ((sync) & 0x30) | ((n_tx_max) & 0x0f));
    m_glossy_state.header.SetRelayCnt(0);
    
    if (m_lwb && m_lwb->GetPHY())
    {
        m_lwb->GetPHY()->SwitchStateSTANDBY();
    }
    
    if (m_glossy_state.header.GetInitiatorId() == m_node->GetId())
    {
        /*  Glossy initiator    */
        if (((m_glossy_state.header.GetPktType() & 0x30) == GLOSSY_UNKNOWN_SYNC) ||
            ((m_glossy_state.payload_len + m_glossy_state.header.GetSerializedSize() + 1) > RF_CONF_MAX_PKT_LEN))
        {
            /** 
             * the initiator must know whether there will be synchronization or
             * not and the packet length may not exceed the max length 
             */
            Stop();
        }
        else
        {
            /*  start the first transmission    */
            m_glossy_state.t_timeout = Simulator::Now().GetSeconds() + m_timeout_delay_seconds;
            
            if (m_lwb && m_lwb->GetPHY())
            {
                new_header = m_glossy_state.header;
                new_header.SetPacketLen(m_glossy_state.payload_len + new_header.GetSerializedSize());
                packet->AddHeader(new_header);
                m_lwb->GetPHY()->Send(packet);
            }
            
            m_glossy_state.relay_cnt_timeout = 0;
        }
    }
    else
    {
        /*  Glossy receiver */
        
        //add glossy receive stuff if needded
        
    }
    
    //while(RF1AIN & BIT0);
}
 
uint8_t
Glossy::Stop(void)
{
    NS_LOG_FUNCTION_NOARGS();
    
    if (m_glossy_state.active)
    {
        //stop the timeout
        
        m_glossy_state.active = 0;
        
        if (m_glossy_state.t_ref_updated)
        {
            if (m_glossy_state.n_T_slot > 0)
            {
                m_glossy_state.t_ref -= (m_glossy_state.relay_cnt_t_ref * m_glossy_state.T_slot_sum) /
                                        m_glossy_state.n_T_slot;
            }
            else
            {
                m_glossy_state.t_ref -= m_glossy_state.relay_cnt_t_ref * m_glossy_state.T_slot_estimated;
            }
        }
    }
    
    return m_glossy_state.n_rx;
}

bool
Glossy::ProcessGlossyHeader(Ptr<Packet> packet)
{
    GlossyHeader header;
    packet->PeekHeader(header);
    
    if (!m_glossy_state.header_ok)
    {
        if ((header.GetPktType() & 0xc0) != GLOSSY_COMMON_HEADER)
        {
            return false;
        }
        
        if (((m_glossy_state.header.GetPktType() & 0x30) != GLOSSY_UNKNOWN_SYNC) &&
            ((m_glossy_state.header.GetPktType() & 0x30) != (header.GetPktType() & 0x30)))
        {
            return false;
        }
        
        if (((m_glossy_state.header.GetPktType() & 0x0f) != GLOSSY_UNKNOWN_N_TX_MAX) &&
            ((m_glossy_state.header.GetPktType() & 0x30) != (header.GetPktType() & 0x30)))
        {
            return false;
        }
        
        if ((m_glossy_state.header.GetInitiatorId() != GLOSSY_UNKNOWN_INITIATOR) &&
            (m_glossy_state.header.GetInitiatorId() != header.GetInitiatorId()))
        {
            return false;
        }
        
        if ((m_glossy_state.payload_len != GLOSSY_UNKNOWN_PAYLOAD_LEN) && 
            (m_glossy_state.payload_len != (packet->GetSize() - 
            ((((m_glossy_state.header.GetPktType()) & 0x30) == GLOSSY_WITHOUT_SYNC) ? 3 : 4))))
        {
            return false;
        }
        
        m_glossy_state.header_ok = 1;
    }
    
    m_glossy_state.header = header;
    m_glossy_state.payload_len = packet->GetSize() - ((((m_glossy_state.header.GetPktType()) & 0x30) ==
                                GLOSSY_WITHOUT_SYNC) ? 3 : 4);
    
    return true;
}

void
Glossy::RxHandler(Ptr<Packet> packet)
{
    m_glossy_state.t_rx_stop = (uint64_t) Simulator::Now().GetSeconds();
    
    if (ProcessGlossyHeader(packet))
    {
        //get payload?
        
        /*  inc relay counter*/
        m_glossy_state.header.SetRelayCnt(m_glossy_state.header.GetRelayCnt() + 1);
        
        if (((m_glossy_state.header.GetPktType() & 0x0f) == 0) ||
            (m_glossy_state.n_tx < (m_glossy_state.header.GetPktType() & 0x0f)))
        {
            /*  retransmit  */
            m_lwb->GetPHY()->Send(packet);
        }
        else
        {
            Stop();
        }
        
        m_glossy_state.n_rx++;
        
        if ((m_glossy_state.header.GetInitiatorId() == m_lwb->GetId()) && (m_glossy_state.n_rx == 1))
        {
            /*  this node is a receiver and this is the first reception */
            //
        }
        
        if ((m_glossy_state.header.GetPktType() & 0x30) == GLOSSY_WITH_SYNC)
        {
            m_glossy_state.relay_cnt_last_rx = m_glossy_state.header.GetRelayCnt() - 1;
            
            if (m_glossy_state.t_ref_updated == 0)
            {
               //m_glossy_state.t_ref = m_glossy_state.t_rx_start - NS_TO_RTIMER_HF(TAU1);
                m_glossy_state.t_ref_updated = 1;
                m_glossy_state.relay_cnt_t_ref = m_glossy_state.header.GetRelayCnt() - 1;
            }
            
            if ((m_glossy_state.relay_cnt_last_rx == m_glossy_state.relay_cnt_last_tx + 1) &&
                (m_glossy_state.n_tx > 0))
            {
                //meaesure Tslot
            }
        }
        
        NS_LOG_INFO("Node #" << m_lwb->GetId() << ": Received Packet(#" << packet->GetUid() << ")");
    }
    else
    {
        //some header fields are wrong
    }
    
    return;
}

uint8_t
Glossy::IsActive(void) const
{
    return m_glossy_state.active;
}

uint8_t
Glossy::GetNRx(void) const
{
    return m_glossy_state.n_rx;
}

uint8_t
Glossy::GetNTx(void) const
{
    return m_glossy_state.n_tx;
}

uint8_t
Glossy::GetPayloadLength(void) const
{
    return m_glossy_state.payload_len;
}

void
Glossy::SetNode(Ptr<Node> node)
{
    m_node = node;
    
    return;
}

Ptr<Node>
Glossy::GetNode(void) const
{
    return m_node;
}

void
Glossy::SetLWB(Ptr<LWB> lwb)
{
    m_lwb = lwb;
    
    return;
}

Ptr<LWB>
Glossy::GetLWB(void) const
{
    return m_lwb;
}

uint8_t
Glossy::IsTRefUpdated(void) const
{
    return m_glossy_state.t_ref_updated;
}

uint64_t
Glossy::GetTRef(void) const
{
    return m_glossy_state.t_ref;
}

void
Glossy::SetTimeoutDelay(double timeout)
{
    m_timeout_delay_seconds = timeout;
    return;
}

double
Glossy::GetTimeoutDelay(void) const
{
    return m_timeout_delay_seconds;
}

}
}
