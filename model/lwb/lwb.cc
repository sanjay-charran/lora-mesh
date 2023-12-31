/*
 * Copyright (c) 2016, Swiss Federal Institute of Technology (ETH Zurich).
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
 * Author:  Reto Da Forno
 *          Federico Ferrari
 *          Marco Zimmerling
 */

#include "ns3/log.h"

#include "ns3/lwb.h"

namespace ns3 {
namespace lora_mesh {
        
NS_LOG_COMPONENT_DEFINE("LWB");
    
TypeId 
LWB::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LWB")
        .SetParent<Object>()
        .SetGroupName("lora_mesh");
        
    return tid;
}

LWB::LWB()
{
    m_schedule_sack = false;
    m_schedule_cont = true;
    m_schedule_dack = false;
    m_schedule_delay = LWB_CONF_SCHED_PERIOD_IDLE;
    m_data_delay = 0.5;
    m_time = Seconds(0);
    m_isInitiator = false;
}

LWB::~LWB()
{
}

void
LWB::Start(void)
{
    NS_LOG_FUNCTION_NOARGS();
    
    //init schedule
    m_schedule.SetPeriod(m_schedule_delay);
    m_schedule.SetNSlots(0);
    m_schedule.SetCONT(m_schedule_cont);
    m_schedule.SetSACK(m_schedule_sack);
    m_schedule.SetDACK(m_schedule_dack);
    
    m_isInitiator = true;
    
    Schedule();
    
    
    return;
}

void 
LWB::Schedule(void)
{
    LWBHeader header;
    Time delay;
    m_schedule.SetTime(Simulator::Now().GetSeconds());
    
    header.SetPacketType(SCHEDULE);
    
    Ptr<Packet> schedule_packet = Create<Packet>(50);
    schedule_packet->AddHeader(m_schedule);
    schedule_packet->AddHeader(header);
    
    //  glossy send schedule
    if (m_glossy)
    {
        m_glossy->StartInitiator(schedule_packet, GLOSSY_ONLY_RELAY_CNT, GLOSSY_WITHOUT_RF_CAL);
    }
    
    delay = Seconds(m_schedule_delay + (m_data_delay * (m_schedule.GetNSlots() +
                ((m_schedule.HasSACK())?1:0) +((m_schedule.HasCONT())?1:0))));
    
    Simulator::Schedule(delay, &LWB::Schedule, this);
    
    return;
}

lwb_conn_state_t 
LWB::GetState(void) const
{
    NS_LOG_FUNCTION_NOARGS();
    
    if (m_sync_state < SYNCED)  /*  BOOTSTRAP or QUASI_SYNCED   */
    {
        return LWB_STATE_INIT;
    }
    else if (m_sync_state < MISSED) /*  SYNCED or SYNCED_2  */
    {
        return LWB_STATE_CONNECTED;
    }
    
    return LWB_STATE_CONN_LOST; /*  UNSYNCED or UNSYNCED2   */
}

void
LWB::Pause(void)
{
    //cancel timers
    return;
}

void
LWB::Resume(void)
{
    //start timers
    return;
}

void
LWB::Send(Ptr<Packet> packet)
{
    /*  Add to queue    */
    m_packet_queue.push(packet);
    
    return;
}
    
void
LWB::DoSend(void)
{
    Ptr<Packet> packet;
    
    if (!m_packet_queue.empty())
    {
        packet = m_packet_queue.front();
        m_packet_queue.pop();
        
        NS_LOG_INFO("Node #" << m_node->GetId() << ": LWB Sending Packet #" << packet->GetUid());
        
        m_glossy->StartInitiator(packet, GLOSSY_WITHOUT_SYNC, GLOSSY_WITHOUT_RF_CAL);
    }
    else 
    {
        NS_LOG_INFO("Node #" << m_node->GetId() << ": Packet Queue Empty");
    }
    
    return;
}
    
void
LWB::Receive(Ptr<Packet> packet)
{
    Ptr<Packet> new_packet;
    GlossyHeader gheader;
    LWBSchedulePacketHeader sheader;
    LWBStreamRequestHeader cheader;
    LWBDataPacketHeader     dheader;
    LWBHeader header, new_header;
    Time cur = Simulator::Now();
    uint16_t slot[LWB_MAX_DATA_SLOTS];
    unsigned int i;
    Time delay;
    
    packet->RemoveHeader(gheader);
    packet->RemoveHeader(header);
    
    switch (header.GetPacketType())
    {
        case SCHEDULE:    
            
            packet->PeekHeader(sheader);
            
            if (sheader.GetTime() != m_schedule.GetTime())
            {
                NS_LOG_INFO("Node #" << m_node->GetId() << ": Received New Schedule");
                
                /*  different schedule --- update actions  */
                m_schedule.SetTime(sheader.GetTime());
                m_schedule.SetPeriod(sheader.GetPeriod());
                m_schedule.SetNSlots(sheader.GetNSlots());
                sheader.GetSlots(slot);
                m_schedule.SetSlots(slot);
                
                for (i = 0;i < m_schedule.GetNSlots();i++)
                {
                    if (slot[i] == m_node->GetId())
                    {
                        /*  this node is scheduled for position i    */
                        
                        delay = Seconds(((i + 1 + ((m_schedule.HasSACK())?1:0)) * m_data_delay) -
                                (cur.GetSeconds() - m_schedule.GetTime()));
                        
                        Simulator::Schedule(delay, &LWB::DoSend, this);
                                
                        break;
                    }
                }
                
                if ((i == m_schedule.GetNSlots()) && m_schedule.HasCONT() && (!m_isInitiator) &&
                    (m_schedule.GetNSlots() <= LWB_MAX_DATA_SLOTS))
                {
                    /*  node not in schedule    */
                    new_packet = Create<Packet>(50);
                    
                    cheader.SetNodeId(m_node->GetId());
                    new_packet->AddHeader(cheader);
                    
                    new_header.SetPacketType(STREAM_REQUEST);
                    new_packet->AddHeader(new_header);
                    
                    Send(new_packet);   //adds cont packet to queue
                    
                    delay = Seconds(((m_schedule.GetNSlots() + 
							((m_schedule.HasSACK())?1:0) + 1) * m_data_delay) - 
							(cur.GetSeconds() - m_schedule.GetTime()));
                    
                    Simulator::Schedule(delay, &LWB::DoSend, this);
                }
            }
            
            delay = Seconds((m_data_delay * (m_schedule.GetNSlots() + ((m_schedule.HasSACK())?1:0) +
                    ((m_schedule.HasCONT())?1:0))) - (cur.GetSeconds() - m_schedule.GetTime()) - 1);
            
            /*  stop glossy after scheduled slots   */
            Simulator::Schedule(delay, &Glossy::Stop, m_glossy);
            
            delay = Seconds(m_schedule_delay + (m_data_delay * (m_schedule.GetNSlots() + 
                    ((m_schedule.HasSACK())?1:0) + ((m_schedule.HasCONT())?1:0)))
                    - (cur.GetSeconds() - m_schedule.GetTime()) - 1);
            
            /*  resume it in time for next schedule */
            Simulator::Schedule(delay, &Glossy::StartReceiver, m_glossy);
            
            break;
                                
        case DATA:    
            
            new_packet = packet->Copy();
            new_packet->RemoveHeader(dheader);
            
            if (dheader.GetRecipientId() == m_node->GetId())
            {
                m_node->GetDevice(0)->GetObject<LoRaNetDevice>()->Receive(new_packet);
            }
            
            break;
                                
        case STREAM_REQUEST:    
            
            packet->PeekHeader(cheader);
            
            if (m_isInitiator)
            {
                m_schedule.GetSlots(slot);
                
                for (i = 0;i < m_schedule.GetNSlots();i++)
                {
                    if (slot[i] == cheader.GetNodeId())
                    {
                        /*  Node already added to network   */
                        break;
                    }
                }
                
                if ((i == m_schedule.GetNSlots()) && m_schedule.HasCONT() && 
                    (m_schedule.GetNSlots() < LWB_MAX_DATA_SLOTS))
                {
                    NS_LOG_INFO("Node #" << m_node->GetId() << "(Scheduler): Added Node #" << 
                                cheader.GetNodeId() << "to Shedule");
                    
                    /*  add node to network */
                    slot[i] = cheader.GetNodeId();
                    m_schedule.SetSlots(slot);
                    m_schedule.SetNSlots(m_schedule.GetNSlots() + 1);
                    
                    /*  schedule SACK is needed */
                    if (m_schedule.HasSACK())
                    {
                        //
                    }
                }
                
            }
            
            break;
                                
        case STREAM_ACK:        
            break;
                                
        case DATA_ACK:          
            break;
    }
    
    packet->AddHeader(header);
    packet->AddHeader(gheader);
    m_glossy->RxHandler(packet);
    
    return;
}

uint8_t
LWB::GetReceiveBufferState(void)
{
    //
    return 0;
}

uint8_t
LWB::GetSendBufferState(void)
{
    //
    return 0;
}

void
LWB::SetGlossy(Ptr<Glossy> glossy)
{
    m_glossy = glossy;
    return;
}

Ptr<Glossy>
LWB::GetGlossy(void) const
{
    return m_glossy;
}

void
LWB::SetNode(Ptr<Node> node)
{
    m_node = node;
    return;
}

Ptr<Node>
LWB::GetNode(void) const
{
    return m_node;
}

void
LWB::SetScheduleDelay(double delay)
{
    m_schedule_delay = delay;
    return;
}

double
LWB::GetScheduleDelay(void) const
{
    return m_schedule_delay;
}

void
LWB::SetDataDelay(double delay)
{
    m_data_delay = delay;
    return;
}

double
LWB::GetDataDelay(void) const
{
    return m_data_delay;
}

}
}
