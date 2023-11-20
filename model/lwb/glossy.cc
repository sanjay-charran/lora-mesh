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
    m_active = false;
    m_n_tx_max = 0;
    m_n_tx = 0;
    m_Tslot = 0;
    m_Tref = 0;
}
 
Glossy::~Glossy()
{
    Stop();
}

void
Glossy::Start(uint16_t initiator_id, Ptr<Packet> packet,
              glossy_sync_t sync, glossy_rf_cal_t rf_cal)
{
    NS_LOG_FUNCTION (this << initiator_id << packet << sync << rf_cal);
    
    GlossyHeader new_header;

    m_active = true;
    uint8_t payload_len = packet->GetSize() - new_header.GetSerializedSize();
    m_n_rx = 0;
    m_T_slot = 0;
    
    /*  prepare the Glossy header    */
    new_header.SetInitiatorId(initiator_id);
    new_header.SetHeaderType(GLOSSY_COMMON_HEADER);
    new_header.SetSync(sync);
    new_header.SetNTxMax(m_n_tx_max);
    new_header.SetRelayCnt(0);
    
    if (m_lwb && m_lwb->GetPHY())
    {
        m_lwb->GetPHY()->SwitchStateSTANDBY();
    }
    
    /*  Glossy initiator    */
    if (initiator_id == m_node->GetId())
    {
        if ((new_header.GetSync() == GLOSSY_UNKNOWN_SYNC) || 
            ((packet->GetSize() + 1) > RF_CONF_MAX_PKT_LEN))
        {
            /** 
             * the initiator must know whether there will be synchronization or
             * not and the packet length may not exceed the max length 
             */
            Stop();
            return;
        }
        else
        {
            /*  start the first transmission    */
            //m_glossy_state.t_timeout = Simulator::Now().GetSeconds() + m_timeout_delay_seconds;
            
            if (m_lwb && m_lwb->GetPHY())
            {
                new_header.SetPacketLen(packet->GetSize());
                packet->AddHeader(new_header);
                
                /*  calc Tslot from difference in time for sending  */
                m_T_ref = Simulator::Now().GetSeconds();
                m_lwb->GetPHY()->Send(packet);
                m_T_slot = Simulator::Now().GetSeconds() - m_Tref;
            }
            
            
            //m_glossy_state.relay_cnt_timeout = 0;
            m_n_tx = 0;
            
            if (n_tx_max)
            {
                //m_last_event = Simulator::Schedule(Seconds(m_timeout_delay_seconds), &Glossy::RxHandler, this, packet);
                m_last_packet= packet;
            }
        }
    }
    else
    {
        /*  Glossy receiver */
        
        //glossy receiver init (shouldn't be anything)
        
    }
    
    return;
}
 
uint8_t
Glossy::Stop(void)
{
    NS_LOG_FUNCTION(this);
    
    if (m_active)
    {
        //stop the timeout
        
        m_active = false;
        
//         if (m_glossy_state.t_ref_updated)
//         {
//             if (m_glossy_state.n_T_slot > 0)
//             {
//                 m_glossy_state.t_ref -= (m_glossy_state.relay_cnt_t_ref * m_glossy_state.T_slot_sum) /
//                                         m_glossy_state.n_T_slot;
//             }
//             else
//             {
//                 m_glossy_state.t_ref -= m_glossy_state.relay_cnt_t_ref * m_glossy_state.T_slot_estimated;
//             }
//         }
    }
    
    return m_n_rx;
}

bool
Glossy::ProcessGlossyHeader(Ptr<Packet> packet)
{
    GlossyHeader header;
    packet->PeekHeader(header);
    
//     if (!m_glossy_state.header_ok)
//     {
//         if ((header.GetPktType() & 0xc0) != GLOSSY_COMMON_HEADER)
//         {
//             return false;
//         }
//         
//         if (((m_glossy_state.header.GetPktType() & 0x30) != GLOSSY_UNKNOWN_SYNC) &&
//             ((m_glossy_state.header.GetPktType() & 0x30) != (header.GetPktType() & 0x30)))
//         {
//             return false;
//         }
//         
//         if (((m_glossy_state.header.GetPktType() & 0x0f) != GLOSSY_UNKNOWN_N_TX_MAX) &&
//             ((m_glossy_state.header.GetPktType() & 0x30) != (header.GetPktType() & 0x30)))
//         {
//             return false;
//         }
//         
//         if ((m_glossy_state.header.GetInitiatorId() != GLOSSY_UNKNOWN_INITIATOR) &&
//             (m_glossy_state.header.GetInitiatorId() != header.GetInitiatorId()))
//         {
//             return false;
//         }
//         
//         if ((m_glossy_state.payload_len != GLOSSY_UNKNOWN_PAYLOAD_LEN) && 
//             (m_glossy_state.payload_len != (packet->GetSize() - 
//             ((((m_glossy_state.header.GetPktType()) & 0x30) == GLOSSY_WITHOUT_SYNC) ? 3 : 4))))
//         {
//             return false;
//         }
//         
//         m_glossy_state.header_ok = 1;
//     }
//     
//     m_glossy_state.header = header;
//     m_glossy_state.payload_len = packet->GetSize() - ((((m_glossy_state.header.GetPktType()) & 0x30) ==
//                                 GLOSSY_WITHOUT_SYNC) ? 3 : 4);
    
    return true;
}

void
Glossy::RxHandler(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);
    
    GlossyHeader new_header;
    Ptr<Packet> new_packet;
    
    if (/*ProcessGlossyHeader(packet)*/m_active)    // temp
    {
        //get payload?
        
        m_n_rx++;
        
        /*  inc relay counter*/
        new_packet =  = packet->Copy();
        new_packet->RemoveHeader(new_header);
        new_header.SetRelayCnt(new_header.GetRelayCnt() + 1);
        new_packet->AddHeader(new_header);
        
        if ((new_header.GetNTxMax() == 0) ||
            (m_n_tx < m_n_tx_max))
        {
            /*  retransmit and calc Tref and Tslot */
            m_T_ref = Simulator::Now();
            m_lwb->GetPHY()->Send(packet);
            m_T_slot = Simulator::Now() - m_T_ref;
            m_T_ref -= m_T_slot * new_header.GetRelayCnt();
            
            m_n_tx++;
            m_last_packet = packet;
            
//             if (m_n_tx < m_n_tx_max)
//             {
//                 if (!m_last_event.IsExpired())
//                 {
//                     if (packet->GetUid() != m_last_packet_id)
//                     {
//                         m_last_event.Cancel();
//                         m_last_event = Simulator::Schedule(Seconds(m_timeout_delay_seconds), &Glossy::RxHandler, this, packet);
//                         m_n_tx = 0;
//                     }
//                 }
//                 else
//                 {
//                     m_last_event = Simulator::Schedule(Seconds(m_timeout_delay_seconds), &Glossy::RxHandler, this, packet);
//                 }
//             }
            
//             m_last_packet_id = packet->GetUid();
        }
        else
        {
            //m_glossy_state.n_tx = 0;
            Stop();
        }
        
//         if (new_header.GetSync() == GLOSSY_WITH_SYNC)
//         {
//             m_glossy_state.relay_cnt_last_rx = m_glossy_state.header.GetRelayCnt() - 1;
//             
//             if (m_glossy_state.t_ref_updated == 0)
//             {
//                m_glossy_state.t_ref = m_glossy_state.t_rx_start - NS_TO_RTIMER_HF(TAU1);
//                 m_glossy_state.t_ref_updated = 1;
//                 m_glossy_state.relay_cnt_t_ref = m_glossy_state.header.GetRelayCnt() - 1;
//             }
//             
//             if ((m_glossy_state.relay_cnt_last_rx == m_glossy_state.relay_cnt_last_tx + 1) &&
//                 (m_n_tx > 0))
//             {
//                 meaesure Tslot
//             }
//         }
        
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
    return m_active;
}

uint8_t
Glossy::GetNRx(void) const
{
    return m_n_rx;
}

uint8_t
Glossy::GetNTx(void) const
{
    return m_n_tx;
}

uint8_t
Glossy::GetPayloadLength(void) const
{
    GlossyHeader temp;
    
    if (m_last_packet)
    {
        return (m_last_packet->GetSize() - temp.GetSerializedSize());
    }
    else
    {
        return 0;
    }
    
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


uint64_t
Glossy::GetTRef(void) const
{
    return m_T_ref;
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

void
Glossy::SetNTxMax(uint8_t n_tx_max)
{
    if (n_tx_max < 1)
    {
        m_n_tx_max = 1;
    }
    else
    {
        m_n_tx_max = n_tx_max;
    }
    return;
}

uint8_t
Glossy::GetNTxMax(void) const
{
    return m_n_tx_max;
}

}
}
