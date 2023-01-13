/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/simulator.h"

#include "ns3/lora-mac.h"

namespace ns3 {
namespace lora_mesh {
 
NS_LOG_COMPONENT_DEFINE ("LoRaMAC");
    
TypeId 
LoRaMAC::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::LoRaMAC")
        .SetParent<Object> ()
        .SetGroupName ("lora_mesh");
        
    return tid;
}

LoRaMAC::LoRaMAC ()
{
    RoutingTableEntry first_entry = {GetId(), GetId(), 0, 0};
    AddTableEntry(first_entry);
    
    m_cur = m_table.begin();
    m_last_counter = 0;
}

LoRaMAC::~LoRaMAC ()
{
}

void
LoRaMAC::SetPHY (Ptr<LoRaPHY> phy)
{
    LoRaMeshHeader header;
    LoRaMeshRoutingHeader rheader;
    
    if (!m_phy)
    {
        m_phy = phy;
        
        /*Time dur_packet, dur_routing;
        Ptr<Packet> max_packet = Create<Packet>(MAX_PACKET_MSG_LENGTH_BYTES);
        
        max_packet->AddHeader(header);
        dur_packet = m_phy->GetOnAirTime(max_packet);
        
        Ptr<Packet> routing_packet = Create<Packet>(0);
        
        
        routing_packet->AddHeader(rheader);
        dur_routing = m_phy->GetOnAirTime(routing_packet);
        
        m_max_packet_time = dur_packet.GetSeconds();
        m_max_routing_time = dur_routing.GetSeconds();
        
        dur_packet = Seconds((m_max_packet_time + m_max_routing_time) * GetId());
        dur_routing = Seconds((m_max_packet_time + m_max_routing_time) * GetId() + m_max_packet_time);
    
        Simulator::Schedule (dur_packet, &LoRaMAC::PacketTimeslot, this);
        Simulator::Schedule (dur_routing, &LoRaMAC::RoutingTimeslot, this);*/
        
        PacketTimeslot();
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
    m_device = device;
    return;
}

Ptr<LoRaNetDevice> 
LoRaMAC::GetDevice(void) const
{
    return m_device;
}

uint32_t 
LoRaMAC::GetId (void) const
{
    if (m_device)
    {
        return m_device->GetNode()->GetId();
    }
    
    return 0;
}

void 
LoRaMAC::AddTableEntry (RoutingTableEntry entry)
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
LoRaMAC::RemoveTableEntry (uint32_t s, uint32_t r)
{
    NS_LOG_FUNCTION (this << s << r);
    
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
LoRaMAC::UpdateTableEntry (RoutingTableEntry entry)
{
    NS_LOG_FUNCTION (this << entry.s << entry.r);
    
    std::deque<RoutingTableEntry>::iterator it = m_table.begin();
    
    if (m_table.empty())
    {
        /*  update by adding    */
        AddTableEntry (entry);
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
    AddTableEntry (entry);
    return;
}

bool
LoRaMAC::EntryExists (RoutingTableEntry entry)
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
    RoutingTableEntry err = {GetId(), GetId(), 1, 0}; /*  an error return since it ought to be 0 etx for node to transmit to itself   */
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

RoutingTableEntry 
LoRaMAC::TableLookup (uint64_t n)
{
    RoutingTableEntry err = {GetId(), GetId(), 1, 0}; /*  an error return since it ought to be 0 etx for node to transmit to itself   */
    std::deque<RoutingTableEntry>::iterator it = m_table.begin();
    
    if (m_table.size() <= n)
    {
        /*  number not in valid range */
        return err;
    }
    
    std::advance (it, n);
    return (*it);
}

void 
LoRaMAC::Receive (Ptr<Packet> packet)
{
    LoRaMeshHeader header;
    LoRaMeshRoutingHeader rheader;
    LoRaMeshFeedbackHeader fheader;
    Ptr<Packet> feedback;
    packet->PeekHeader(header);
    RoutingTableEntry entry, temp;
    
    switch (header.GetType())
    {
        case ROUTING_UPDATE:
            packet->RemoveHeader(header);
            packet->RemoveHeader(rheader);
            
            entry.s = header.GetSrc();
            entry.r = header.GetDest();
            entry.etx = rheader.GetETX();
            entry.last = rheader.GetLast();
            
            if (EntryExists(entry))
            {
                temp = TableLookup(header.GetFwd(), GetId());
                UpdateTableEntry(entry);
                
                if (!IsErrEntry(temp))
                {
                    temp.etx = (entry.last > temp.last)?(1.0/(entry.last - temp.last)):(1.0/((255 - temp.last) + entry.last + 1));
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
                    entry.etx = 1.0/(entry.last + 1);
                    entry.r = GetId();  /*  node broadcasting itself was received so adds that as the entry     */
                }
                
                if (entry.etx <= 10)
                {
                    AddTableEntry(entry);
                }
            }
            
            
            break;
        case BROADCAST:
            //unsure if to implement
            break;
        case DIRECTED:
            
            if (header.GetDest() == GetId())
            {
                feedback = MakeFeedback (packet, header.GetFwd());
                AddPacketToQueue (feedback, true);
                packet->RemoveHeader(header);
                m_device->Receive(packet);
                break;
            }
            
            /*  forward if not recipient    */
            if (CalcETX(GetId(), header.GetDest()) < CalcETX(header.GetFwd(), header.GetDest()))
            {
                feedback = MakeFeedback (packet, header.GetFwd());
                AddPacketToQueue (feedback, true);
                packet->RemoveHeader(header);            
                header.SetFwd(GetId());
                packet->AddHeader(header);
                AddPacketToQueue (packet, false);
            }
            
            break;
        case FEEDBACK:
            
            packet->RemoveHeader(header);
            packet->RemoveHeader(fheader);
            
            if (header.GetDest() == GetId())
            {
                if (fheader.GetPacketId() == GetNextPacketFromQueue()->GetUid() || /*  for feedback for packets    */
                    packet->GetUid() == GetNextPacketFromQueue()->GetUid())        /*  for feedback for feedback */
                {
                    RemovePacketFromQueue();
                }
            }
            
            if (CalcETX(header.GetFwd(), header.GetDest()) > CalcETX(GetId(), header.GetDest()))
            {
                packet->AddHeader(fheader);
                header.SetFwd(GetId());
                packet->AddHeader(header);
                AddPacketToQueue(packet, true);
            }
            
            break;
    }
}

void
LoRaMAC::Broadcast (Ptr<Packet> packet)
{
    LoRaMeshHeader header;
    
    if (m_phy)
    {
        header.SetType(BROADCAST);
        header.SetSrc(GetId());
        /*  broadcast doesnt have specific destination node so no need to set it    */
        
        packet->AddHeader(header);
        AddPacketToQueue (packet, false);
    }
    
    return;
}

void 
LoRaMAC::SendTo (Ptr<Packet> packet, uint32_t dest)
{
    NS_LOG_FUNCTION (this << packet << dest);
    
    /*  send from attached node to dest node ID */
    LoRaMeshHeader header;
    
    if (m_phy)
    {
        header.SetType(DIRECTED);
        header.SetSrc(GetId());
        header.SetDest(dest);
        
        packet->AddHeader(header);
        AddPacketToQueue (packet, false);
    }
    
    return;
}

void
LoRaMAC::AddPacketToQueue (Ptr<Packet> packet, bool isFeedback)
{
    NS_LOG_FUNCTION (this << packet << isFeedback);
    
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
LoRaMAC::RemovePacketFromQueue (void)
{
    NS_LOG_FUNCTION (this);
    
    if (!m_packet_queue.empty())
    {
       m_packet_queue.pop_front();
    }

    return;
}

Ptr<Packet> 
LoRaMAC::GetNextPacketFromQueue (void)
{
    return m_packet_queue.front();
}

void 
LoRaMAC::AddToLastPacketList (Ptr<Packet> packet)
{
    m_last_packets.push_back(packet->GetUid());
    
    if (m_last_packets.size() > MAX_NUMEL_LAST_PACKETS_LIST)
    {
        m_last_packets.pop_front();
    }
    
    return;
}

bool 
LoRaMAC::SearchLastPacketList (Ptr<Packet> packet)
{
    unsigned int i;
    uint64_t id = packet->GetUid();
    
    for (i = 0;i < MAX_NUMEL_LAST_PACKETS_LIST;i++)
    {
        if (m_last_packets[i] == id)
        {
            return true;
        }
    }
    
    return false;
}

float
LoRaMAC::CalcETX (uint32_t src, uint32_t dest)
{
    return CalcETX(src, dest, src);
}

/*  routing algorithm -- improve later  */
float
LoRaMAC::CalcETX (uint32_t src, uint32_t dest, uint32_t last)
{
    float etx, min;
    std::deque<RoutingTableEntry>::iterator it;
    
    if (src == dest)
    {
        return 0.0;
    }
    
    
    for (it = m_table.begin();it != m_table.end();++it)
    {
        if (it->s == src && it->r == dest)
        {
            return it->etx;
        }
        
        if (it->s > src)
        {
            /*  beyond where the entry ought to be  */
            break;
        }
    }
    
    for (it = m_table.begin(), min = 0;it != m_table.end();++it)
    {
        if (it->s == src && (!EntryExists(TableLookup(last, it->r)) || src == last))
        {
            etx = CalcETX(it->r, dest, src) + it->etx;
            
            if (etx < min || min == 0)
            {
                min = etx;
            }
        }
    }
    
    return min;
}

Ptr<Packet>
LoRaMAC::MakeFeedback (Ptr<Packet> packet, uint32_t fwd)
{
    Ptr<Packet> feedback = Create<Packet>();
    LoRaMeshFeedbackHeader fheader;
    LoRaMeshHeader header;
    
    fheader.SetPacketId(packet->GetUid());
    feedback->AddHeader(fheader);
    
    header.SetSrc(GetId());
    header.SetDest(fwd);
    header.SetType(FEEDBACK);
    feedback->AddHeader(header);
    
    return feedback;
}

void
LoRaMAC::PacketTimeslot (void)
{
    NS_LOG_FUNCTION (this);
    
    Ptr<Packet> next;
    LoRaMeshHeader header;
    int i;
    unsigned int count;
    uint32_t temp;
    Time dur;
    
    if (!m_packet_queue.empty())
    {
        next = GetNextPacketFromQueue();
        next->PeekHeader(header);
        
        if (CalcETX(GetId(), header.dest) != 0)
        {
            for (i = MAX_NUMEL_LAST_PACKETS_LIST - 1, count = 0;i >= 0;i--)
            {
                if (m_last_packets[i] == next->GetUid())
                {
                    count++;
                }
            }
            
            m_phy->Send(next);
            AddToLastPacketList (next);
            
            if (count == 9)
            {
                /*  remove if it is on tenth send   */
                RemovePacketFromQueue();
            }
            
            /*  schedule routing timeslot after packet timeslot */
            dur = m_phy->GetOnAirTime(packet);
            dur = Seconds(dur.GetSeconds() + 0.1);
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
    temp = x->GetInteger(MIN_RAND_UINT32, MAX_RAND_UINT32);
    dur = Seconds(temp);        //  adjust to be more reasonable based on tested data or make user controlled
    
    
    /*  next timeslot will be after the routing timeslot for this device and the packet and routing slots for all other devices */
    //dur = Seconds((m_max_packet_time + m_max_routing_time) * (m_phy->GetChannel()->GetNDevices() - 1) + m_max_routing_time);
    
    /*  schedule next slot  */
    Simulator::Schedule(dur, &LoRaMAC::PacketTimeslot, this);
    
    return;
}

void
LoRaMAC::RoutingTimeslot (void)
{
    NS_LOG_FUNCTION (this);
    
    Time dur;
    
    Ptr<Packet> packet = Create<Packet>();
    LoRaMeshRoutingHeader rheader;
    LoRaMeshHeader header;
    
    header.SetType(ROUTING_UPDATE);
    header.SetSrc(m_cur->s);
    header.SetDest(m_cur->r);
    header.SetFwd(GetId());
    
    rheader.SetETX(m_cur->etx);
    rheader.SetLast(m_last_counter);
    
    packet->AddHeader(rheader);
    packet->AddHeader(header);
    
    m_phy->Send(packet);
    
    
    /*  move entry iterator by one  */
    m_cur++;
    
    if (m_cur == m_table.end())
    {
        m_cur = m_table.begin();// + std::distance<std::const_iterator>(m_cur, ci);;
    }
    
    /*  increment last counter  */
    if (m_last_counter == 255)  /*  max for uint8_t */
    {
        m_last_counter = 0;
    }
    else
    {
        m_last_counter++;
    }
    
    /*  every other nodes timeslots and the next packet timeslot for this node */
   // dur = Seconds((m_max_packet_time + m_max_routing_time) * (m_phy->GetChannel()->GetNDevices() - 1) + m_max_packet_time);
    
    //Simulator::Schedule(dur, &LoRaMAC::RoutingTimeslot, this);
    
    return;
}

}
}
