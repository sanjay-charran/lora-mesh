/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2023 Sanjay Charran
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Sanjay Charran <sanjaycharran@gmail.com>
 */

#include "ns3/simulator.h"

#include "ns3/lora-mac.h"

namespace ns3 {
namespace lora_mesh {
 
NS_LOG_COMPONENT_DEFINE("LoRaMAC");
    
TypeId 
LoRaMAC::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LoRaMAC")
        .SetParent<Object>()
        .SetGroupName("lora_mesh")
        .AddTraceSource("RxPacketSniffer",
                        "Trace Source which simulates sniffer for received data packets",
                        MakeTraceSourceAccessor(&LoRaMAC::m_rxPacketSniffer),
                        "ns3::LoRaMAC::RxPacketSnifferTracedCallback")
        .AddTraceSource("TxPacketSniffer",
                        "Trace Source which simulates sniffer for transmitted data packets",
                        MakeTraceSourceAccessor(&LoRaMAC::m_txPacketSniffer),
                        "ns3::LoRaMAC::TxPacketSnifferTracedCallback");
        
    return tid;
}

LoRaMAC::LoRaMAC()
{
    m_last_counter = 0;
    m_minDelay = 0;
    m_maxDelay = 60;
    m_routingUpdateFreq = 1;
    m_routingUpdateCounter = 0;
}

LoRaMAC::~LoRaMAC()
{
}

void
LoRaMAC::SetPHY(Ptr<LoRaPHY> phy)
{
    LoRaMeshHeader header;
    LoRaMeshRoutingHeader rheader;
    
    if (!m_phy)
    {
        m_phy = phy;
        
        Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable>();
        double temp = x->GetValue(m_minDelay, m_maxDelay);
        Time dur = Seconds(temp);
        
        Simulator::Schedule(dur, &LoRaMAC::PacketTimeslot, this);
    }
    else
    {
        m_phy = phy;
    }
    
    return;
}

Ptr<LoRaPHY>
LoRaMAC::GetPHY (void) const
{
    return m_phy;
}

void
LoRaMAC::SetDevice (Ptr<LoRaNetDevice> device)
{
    Vector3D pos;
    m_device = device;
    
    
    RoutingTableEntry first_entry;
    first_entry.s = GetId();
    first_entry.r = GetId();
    first_entry.etx = 0;
    first_entry.last = 0;
    
    AddTableEntry(first_entry);
    
    if (m_phy && m_phy->GetMobility())
    {
        pos = m_phy->GetMobility()->GetPosition();
        NS_LOG_INFO("Node #" << GetId() << "(x=" << pos.x << " y=" << pos.y << " z=" << pos.z << ")" << ": Added entry (" << first_entry.s << "->" << first_entry.r << ")");
    }
    
    return;
}

Ptr<LoRaNetDevice> 
LoRaMAC::GetDevice(void) const
{
    return m_device;
}

uint32_t 
LoRaMAC::GetId(void) const
{
    if (m_device)
    {
        return m_device->GetNode()->GetId();
    }
    
    return 0;
}

void
LoRaMAC::SetMinDelay(uint32_t min)
{
    m_minDelay = min;
    return;
}

uint32_t
LoRaMAC::GetMinDelay(void) const
{
    return m_minDelay;
}

void
LoRaMAC::SetMaxDelay(uint32_t max)
{
    m_maxDelay = max;
    return;
}

uint32_t
LoRaMAC::GetMaxDelay(void) const
{
    return m_maxDelay;
}

void
LoRaMAC::SetRoutingUpdateFrequency(uint32_t freq)
{
    if (freq != 0)
    {
        m_routingUpdateFreq = freq;
    }
    
    return;
}

uint32_t
LoRaMAC::GetRoutingUpdateFrequency(void) const
{
    return m_routingUpdateFreq;
}

void 
LoRaMAC::AddTableEntry(RoutingTableEntry entry)
{
    NS_LOG_FUNCTION (this << entry.s << entry.r);
    
    std::deque<RoutingTableEntry>::iterator it = m_table.begin();
    
    if (m_table.empty())
    {
        m_table.insert(it, entry);
        return;
    }
    
    for (;it != m_table.end();++it)
    {
        if (it->s > entry.s)
        {
            m_table.insert(it, entry);
            return;
        }
        else if (it->s == entry.s && it->r > entry.r)
        {
            m_table.insert(it, entry);
            return;
        }
        else if (it->s == entry.s && it->r == entry.r)
        {
            /*  entry already exists -- dont add duplicates */
            return;
        }
    }
    
    m_table.insert(it, entry);
    return;
}

void 
LoRaMAC::RemoveTableEntry(uint32_t s, uint32_t r)
{
    NS_LOG_FUNCTION(this << s << r);
    
    std::deque<RoutingTableEntry>::iterator it = m_table.begin();
    
    if (m_table.empty())
    {
        /*  nothing to remove   */
        return;
    }
    
    for (;it != m_table.end();++it)
    {
        if (it->s == s && it->r == r)
        {
            /*  remove entry    */
            m_table.erase(it);
            return;
        }
    }
    
    /*  did not find matching entry */
    return;
}

void 
LoRaMAC::UpdateTableEntry(RoutingTableEntry entry)
{
    NS_LOG_FUNCTION(this << entry.s << entry.r);
    
    std::deque<RoutingTableEntry>::iterator it = m_table.begin();
    
    if (m_table.empty())
    {
        /*  update by adding    */
        AddTableEntry(entry);
        return;
    }
    
    for (;it != m_table.end();++it)
    {
        if (it->s == entry.s && it->r == entry.r)
        {
            /*  update  */
            it->etx = entry.etx;
            it->last = entry.last;
            return;
        }
    }
    
    /*  could not find -- add in entry  */
    AddTableEntry(entry);
    return;
}

bool
LoRaMAC::EntryExists(RoutingTableEntry entry)
{
    std::deque<RoutingTableEntry>::iterator it = m_table.begin();
    
    if (m_table.empty())
    {
        return false;
    }
    
    for (;it != m_table.end();++it)
    {
        if (it->s == entry.s && it->r == entry.r)
        {
            /*  only checks src and dest equality   */
            return true;
        }
    }
    
    return false;
}

bool
LoRaMAC::IsErrEntry (RoutingTableEntry entry)
{
    if (entry.s == entry.r && entry.etx != 0)
    {
        return true;
    }
    
    return false;
}

RoutingTableEntry 
LoRaMAC::TableLookup (uint32_t s, uint32_t r)
{
    RoutingTableEntry err = {GetId(), GetId(), 1, 0};   /*  an error return since it ought to be 
                                                            0 etx for node to transmit to itself   */
                                                            
    std::deque<RoutingTableEntry>::iterator it = m_table.begin();
    
    for (;it != m_table.end();++it)
    {
        if (it->s == s && it->r == r)
        {
            return (*it);
        }
    }
 
    return err; /*  returns initialised val */
}

void 
LoRaMAC::Receive (Ptr<Packet> packet)
{
    LoRaMeshHeader header;
    LoRaMeshRoutingHeader rheader;
    LoRaMeshFeedbackHeader fheader;
    Ptr<Packet> feedback, new_packet;
    
    Vector3D pos = m_phy->GetMobility()->GetPosition();;
    RoutingTableEntry entry, temp;
    
    packet->PeekHeader(header);
    
    switch (header.GetType())
    {
        case ROUTING_UPDATE:
            packet->RemoveHeader(header);
            packet->RemoveHeader(rheader);
            
            entry.s = header.GetSrc();
            entry.r = header.GetDest();
            entry.etx = rheader.GetETX();
            entry.last = rheader.GetLast();
            
            NS_LOG_INFO("(receive MAC)Node (x=" << pos.x << " y=" << pos.y << " z=" << pos.z << ")#" << header.GetFwd() << "->#" << GetId() << ": " << entry.s << "->" << entry.r << " (etx: " << entry.etx << ")");
            
            if (EntryExists(entry))
            {
                temp = TableLookup(header.GetFwd(), GetId());
                UpdateTableEntry(entry);
                
                if (!IsErrEntry(temp))
                {
                    temp.etx = (entry.last > temp.last)?(entry.last - temp.last):(255 - temp.last + entry.last);
                    temp.last = entry.last;
                    
                    if (temp.etx > 10)
                    {
                        RemoveTableEntry(temp.s, temp.r);
                    }
                    else
                    {
                        UpdateTableEntry(temp); /*  update etx and last  */
                    }
                }
                
            }
            else
            {
                if (entry.s == entry.r)
                {
                    entry.etx = (entry.last + 1);
                    entry.r = GetId();  /*  node broadcasting itself was received so adds that as the entry     */
                }
                
                if (entry.etx <= 10)
                {
                    NS_LOG_INFO("Node #" << GetId() << ": Added entry (" << entry.s << "->" << entry.r << ")");
                    AddTableEntry(entry);
                }
            }
            packet->AddHeader(rheader);
            packet->AddHeader(header);
            
            
            break;
        case DIRECTED:
            
            NS_LOG_INFO("(receive MAC)Node (x=" << pos.x << " y=" << pos.y << " z=" << pos.z << ")#" << header.GetSrc() << "(" << header.GetFwd() << ")->" << GetId() << ": Packet #" << packet->GetUid());
            
            m_rxPacketSniffer(packet);
            
            if (header.GetDest() == GetId())
            {
                feedback = MakeFeedback(packet, header.GetFwd());
                AddPacketToQueue(feedback, true);
                break;
            }
            
            /*  forward if not recipient    */
            if (CalcETX(GetId(), header.GetDest()) < CalcETX(header.GetFwd(), header.GetDest()))
            {
                feedback = MakeFeedback(packet, header.GetFwd());
                AddPacketToQueue(feedback, true);
                new_packet = packet->Copy();
                new_packet->RemoveHeader(header);            
                header.SetFwd(GetId());
                new_packet->AddHeader(header);
                AddPacketToQueue(new_packet, false);
            }
            
            break;
        case FEEDBACK:
            
            packet->RemoveHeader(header);
            packet->RemoveHeader(fheader);
            
            NS_LOG_INFO("(receive MAC)Node (x=" << pos.x << " y=" << pos.y << " z=" << pos.z << ")#" << header.GetFwd() << "->" << GetId() << ": Feedback #" << fheader.GetPacketId());
            
            if (header.GetDest() == GetId() && !m_packet_queue.empty())
            {
                if (isPacketInQueue(fheader.GetPacketId())) /*  for feedback for packets    */
                {
                    packet->AddHeader(fheader);
                    packet->AddHeader(header);
                    RemovePacketFromQueue(fheader.GetPacketId());
                    break;
                }
                else if (isPacketInQueue(packet->GetUid()))
                {
                    packet->AddHeader(fheader);
                    packet->AddHeader(header);
                    RemovePacketFromQueue(packet->GetUid());
                    break;
                }
                
                break;
            }
            
            if (CalcETX(header.GetFwd(), header.GetDest()) > CalcETX(GetId(), header.GetDest()))
            {
                new_packet = packet->Copy();
                new_packet->AddHeader(fheader);
                header.SetFwd(GetId());
                new_packet->AddHeader(header);
                AddPacketToQueue(new_packet, true);
            }
            
            packet->AddHeader(fheader);
            packet->AddHeader(header);
            break;
    }
}

void 
LoRaMAC::Send(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);
    
    if (m_phy)
    {
        AddPacketToQueue(packet, false);
    }
    
    return;
}

void 
LoRaMAC::SendTo(Ptr<Packet> packet, uint32_t dest)
{
    NS_LOG_FUNCTION(this << packet << dest);
    
    /*  send from attached node to dest node ID */
    LoRaMeshHeader header;
    
    if (m_phy)
    {
        header.SetType(DIRECTED);
        header.SetSrc(GetId());
        header.SetDest(dest);
        header.SetFwd(GetId());
        
        packet->AddHeader(header);
        AddPacketToQueue(packet, false);
    }
    
    return;
}

void
LoRaMAC::AddPacketToQueue(Ptr<Packet> packet, bool isFeedback)
{
    NS_LOG_FUNCTION(this << packet << isFeedback);
    
    std::deque<Ptr<Packet>>::iterator it;
    LoRaMeshHeader header;
    
    if (isFeedback)
    {
        for (it = m_packet_queue.begin();it != m_packet_queue.end();++it)
        {
            (*it)->PeekHeader(header);
            
            if (header.GetType() != FEEDBACK)
            {
                m_packet_queue.insert(it, packet);
                return;
            }
        }
        
        m_packet_queue.push_back(packet); /*    only feeedback or empty queue   */
    }
    else
    {
        m_packet_queue.push_back(packet);
    }
    
    return;
}

void
LoRaMAC::RemovePacketFromQueue(uint32_t pid)
{
    NS_LOG_FUNCTION(this << pid);
    
    std::deque<Ptr<Packet>>::iterator it;
    
    for (it = m_packet_queue.begin();it != m_packet_queue.end();++it)
    {
        if ((*it)->GetUid() == pid)
        {
            /*  remove packet if found  */
            m_packet_queue.erase(it);
            return;
        }
    }

    return;
}

bool
LoRaMAC::isPacketInQueue(uint32_t pid)
{
    NS_LOG_FUNCTION(this << pid);
    
    std::deque<Ptr<Packet>>::iterator it;
    
    for (it = m_packet_queue.begin();it != m_packet_queue.end();++it)
    {
        if ((*it)->GetUid() == pid)
        {
            return true;
        }
    }
    
    return false;
}

Ptr<Packet> 
LoRaMAC::GetNextPacketFromQueue(void)
{
    return m_packet_queue.front();
}

void 
LoRaMAC::AddToLastPacketList(Ptr<Packet> packet)
{
    m_last_packets.push_back(packet->GetUid());
    
    if (m_last_packets.size() > MAX_NUMEL_LAST_PACKETS_LIST)
    {
        m_last_packets.pop_front();
    }
    
    return;
}

float
LoRaMAC::CalcETX(uint32_t src, uint32_t dest)
{
    std::vector<RoutingTableEntry> checked_nodes;
    std::deque<RoutingTableEntry>::iterator it, temp;
    float min;
    unsigned int i, j;
    bool flag;
    RoutingTableEntry entry;
    entry.r = src;
    entry.etx = 0;
    
    checked_nodes.push_back(entry);
    
    for (;;)
    {
        for (i = 0, min = 1000000;i < checked_nodes.size();i++)
        {
            for (it = m_table.begin();it != m_table.end();++it)
            {
                if (checked_nodes[i].r == it->s)
                {
                    for (j = 0, flag = false;j < checked_nodes.size();j++)
                    {
                        if (checked_nodes[j].r == it->r)
                        {
                            flag = true;
                            break;
                        }
                    }
                    
                    if (!flag && (checked_nodes[i].etx + it->etx) < min)
                    {
                        min = checked_nodes[i].etx + it->etx;
                        temp = it;
                    }
                }
            }
        }
        
        if (min != 1000000)
        {
            if (temp->r == dest)
            {
                return min;
            }
            
            entry.r = temp->r;
            entry.etx = min;
            checked_nodes.push_back(entry);
        }
        else
        {
            return 0;
        }
        
    }
}

Ptr<Packet>
LoRaMAC::MakeFeedback(Ptr<Packet> packet, uint32_t fwd)
{
    Ptr<Packet> feedback = Create<Packet>(50);
    LoRaMeshFeedbackHeader fheader;
    LoRaMeshHeader header;
    
    fheader.SetPacketId(packet->GetUid());
    feedback->AddHeader(fheader);
    
    header.SetSrc(GetId());
    header.SetDest(fwd);
    header.SetFwd(GetId());
    header.SetType(FEEDBACK);
    feedback->AddHeader(header);
    
    return feedback;
}

void
LoRaMAC::PacketTimeslot(void)
{
    NS_LOG_FUNCTION(this);
    
    Ptr<Packet> next;
    LoRaMeshHeader header;
    int i;
    unsigned int count;
    double temp;
    Time dur;
    Vector3D pos = m_phy->GetMobility()->GetPosition();
    
    if (!m_packet_queue.empty())
    {
        next = GetNextPacketFromQueue();
        next->PeekHeader(header);
        
        if (CalcETX(GetId(), header.GetDest()) != 0 || header.GetType() == FEEDBACK)
        {
            for (i = MAX_NUMEL_LAST_PACKETS_LIST - 1, count = 0;i >= 0;i--)
            {
                if (m_last_packets[i] == next->GetUid())
                {
                    count++;
                }
            }
            
            NS_LOG_INFO("(send MAC)Node #" << GetId() << " (x=" << pos.x << " y=" << pos.y << " z=" << pos.z << "): " << header.GetSrc() << "->" << header.GetDest() << " Packet #" << next->GetUid());
            
            m_phy->Send(next);
            AddToLastPacketList (next);
            
            if (header.GetType() != FEEDBACK)
            {
                m_txPacketSniffer(next);
            }
            
            if (count == 9 || header.GetType() == FEEDBACK)
            {
                /*  remove if it is on tenth send   */
                RemovePacketFromQueue(next->GetUid());
            }
            
            /*  schedule routing timeslot after packet timeslot */
            dur = m_phy->GetOnAirTime(next);
            dur = Seconds(dur.GetSeconds());
            Simulator::Schedule(dur, &LoRaMAC::RoutingTimeslot, this);
        }
        else
        {
            RoutingTimeslot();
        }
    }
    else
    {
        /*  use timeslot for routing if packet queue is empty   */
        RoutingTimeslot();
    }
    
    Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable>();
    temp = x->GetValue(m_minDelay, m_maxDelay);
    dur = Seconds(temp);
    
    /*  schedule next slot  */
    Simulator::Schedule(dur, &LoRaMAC::PacketTimeslot, this);
    
    return;
}

void
LoRaMAC::RoutingTimeslot(void)
{
    NS_LOG_FUNCTION(this);
    
    m_routingUpdateCounter++;
    
    if (m_routingUpdateCounter < m_routingUpdateFreq)
    {
        return;
    }
    
    Time dur;
    Ptr<Packet> packet = Create<Packet>(25);
    LoRaMeshHeader header;
    Vector3D pos = m_phy->GetMobility()->GetPosition();
    
    auto size = m_table.size();
    Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable>();
    uint32_t temp = x->GetInteger(0, size);
    
    RoutingTableEntry cur = m_table[temp];
    
    header.SetType(ROUTING_UPDATE);
    header.SetSrc(cur.s);
    header.SetDest(cur.r);
    header.SetFwd(GetId());
    
    LoRaMeshRoutingHeader rheader;
    rheader.SetETX(cur.etx);
    rheader.SetLast(m_last_counter);
    
    NS_LOG_INFO("(send MAC)Node #" << GetId() << "(x=" << pos.x << " y=" << pos.y << " z=" << pos.z << "): " << cur.s << "->" << cur.r << " (etx: " << cur.etx << ")");
    
    packet->AddHeader(rheader);
    
    packet->AddHeader(header);
    
    m_phy->Send(packet);
    
    /*  increment last counter  */
    if (m_last_counter == 255)  /*  max for uint8_t */
    {
        m_last_counter = 0;
    }
    else
    {
        m_last_counter++;
    }
    
    return;
}

}
}
