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
    m_schedule_delay = LWB_CONF_SCHED_PERIOD_IDLE;
    m_data_delay = 0.5;
    m_time = Seconds(0);
}

LWB::~LWB()
{
}

void
LWB::Start(void)
{
    NS_LOG_FUNCTION_NOARGS();
    
    //init schedule
    m_schedule.SetTime(Simulator::Now().GetSeconds());
    m_schedule.SetPeriod(m_schedule_delay);
    m_schedule.SetNSlots(0);
    
    if (m_schedule_cont)
    {
        m_schedule.SetNSlots(m_schedule.GetNSlots() | LWB_SCHED_CONT_SLOT);
    }
    
    Schedule();
    
    
    return;
}

void 
LWB::Schedule(void)
{
    Ptr<Packet> schedule_packet = Create<Packet>(50);
    packet->AddHeader(m_schedule);
    Time delay;
    
    //  glossy send schedule
    if (m_glossy)
    {
        m_glossy->StartInitiator(schedule_packet, GLOSSY_ONLY_RELAY_CNT, GLOSSY_WITHOUT_RF_CAL);
    }
    
    delay = Seconds(m_schedule_delay + 
            (m_data_delay * ((m_schedule.GetNSlots() & ~(LWB_SCHED_CONT_SLOT | LWB_SCHED_SACK_SLOT)) +
                ((m_schedule.GetNSlots() & LWB_SCHED_SACK_SLOT)?1:0) +
                ((m_schedule.GetNSlots() & LWB_SCHED_CONT_SLOT)?1:0))));
    
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
    /*  placeholder func call   */
    //m_glossy->Start(GetId(), packet, 3, GLOSSY_WITHOUT_SYNC, GLOSSY_WITHOUT_RF_CAL);
    return;
}
    
void
LWB::Receive(Ptr<Packet> packet)
{
    GlossyHeader gheader;
    LWBSchedulePacketHeader sheader;
    Time cur = Simulator::Now();
    
    packet->RemoveHeader(gheader);
    
    if ((m_schedule.GetTime() == 0) ||
        ((cur.GetSeconds() >= (m_time.GetSeconds() + m_schedule.GetPeriod())) &&
            (cur.GetSeconds() < (m_time.GetSeconds() + m_schedule.GetPeriod()) + m_data_delay)))
    {
        //schedule
        m_time = cur;
        packet->PeekHeader(sheader);
        m_schedule = sheader;
        
        
    }
    else
    {
        if ((m_schedule.GetNSlots() & LWB_SCHED_SACK_SLOT) &&
            (cur.GetSeconds() >= (m_time.GetSeconds() + m_data_delay)) &&
            (cur.GetSeconds() < (m_time.GetSeconds() + 2*m_data_delay)))
        {
            //sack
        }
        else if ((m_schedule.GetNSlots() & ~(LWB_SCHED_CONT_SLOT | LWB_SCHED_SACK_SLOT)) &&
                (cur.GetSeconds() >= (m_time.GetSeconds() + 
                        ((m_schedule.GetNSlots() & LWB_SCHED_SACK_SLOT)?2:1)*m_data_delay)) &&
                (cur.GetSeconds() < (m_time.GetSeconds() + m_data_delay *
                        (((m_schedule.GetNSlots() & LWB_SCHED_SACK_SLOT)?2:1) + 
                            (m_schedule.GetNSlots() & ~(LWB_SCHED_CONT_SLOT | LWB_SCHED_SACK_SLOT))))))
        {
            
            //data
        }
        else if ((m_schedule.GetNSlots() & LWB_SCHED_CONT_SLOT) &&
                 ())
        {
            //cont
        }
    }
    
    
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
