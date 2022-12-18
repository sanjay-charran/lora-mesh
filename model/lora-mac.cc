/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"

#include "ns3/lora-mac.h"

namespace ns3 {
namespace lora_mesh {
 
TypeId 
LoRaMAC::GetTypeId (void)
{
    static tid = TypeId ("ns3::LoRaMAC")
        .SetParent<Object> ()
        .SetGroupName ("lora_mesh");
        
    return tid;
}

LoRaMAC::LoRaMAC ()
{
}

LoRaMAC::~LoRaMAC ()
{
}

void
LoRaMAC::SetPHY (Ptr<LoRaPHY> phy)
{
    m_phy = phy;
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
    std::list<RoutingTableEntry>::iterator it = m_table.begin();
    
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
LoRaMAC::RemoveTableEntry (utin16_t s, uint16_t r)
{
    std::list<RoutingTableEntry>::iterator it = m_table.begin();
    
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
    std::list<RoutingTableEntry>::iterator it = m_table.begin();
    
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
            it->etx = entry->etx;
            it->last = entry->last;
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
    std::list<RoutingTableEntry>::iterator it = m_table.begin();
    
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
LoRaMAC::TableLookup (uint16_t s, uint16_t r) const
{
    RoutingTableEntry err = {GetId(), GetId(), 1, 0}; /*  an error return since it ought to be 0 etx for node to transmit to itself   */
    std::list<RoutingTableEntry>::iterator it = m_table.begin();
    
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
LoRaMAC::TableLookup (uint64_t n) const
{
    RoutingTableEntry err = {GetId(), GetId(), 1, 0}; /*  an error return since it ought to be 0 etx for node to transmit to itself   */
    std::list<RoutingTableEntry>::iterator it = m_table.begin();
    
    if (m_table.size() <= n)
    {
        /*  number not in valid range */
        return err;
    }
    
    advance (it, n);
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
                temp = TableLookup(entry.s, GetId());
                UpdateTableEntry(entry);
                
                if (!IsErrEntry(temp))
                {
                    temp.etx = (entry.last > temp.last)?(1.0/(entry.last - temp.last)):(1.0/((255 - temp.last) + entry.last + 1));
                    temp.last = entry.last;
                    
                    UpdateTableEntry(temp); /*  update etx and last  */
                }
                
            }
            else
            {
                AddTableEntry(entry);
            }
            
            
            break;
        case BROADCAST:
            //unsure if to implement
            break;
        case DIRECTED:
            
            if (header.GetDest() == GetId())
            {
                feedback = MakeFeedback (packet);
                AddPacketToQueue (feedback);
                packet->RemoveHeader(header);
                m_device->Receive(packet);
                break;
            }
            
            /*  forward if not recipient    */
            if (CalcETX(GetId(), header.GetDest()) < CalcETX(header.GetFwd(), header.GetDest()))
            {
                feedback = MakeFeedback (packet);
                AddPacketToQueue (feedback);
                packet->RemoveHeader(header);            
                header.SetFwd(GetId());
                packet->AddHeader(header);
                AddPacketToQueue (packet);
            }
            
            break;
        case FEEDBACK:
            
            packet->RemoveHeader(header);
            packet->RemoveHeader(fheader);
            
            if (fheader.GetPacketId() == GetNexPacketFromQueue->GetUid() || /*  for feedback for packets    */
                packet->GetUid() == GetNexPacketFromQueue->GetUid())    /*  for feedback for feedback */
            {
                RemovePacketFromQueue();
            }
            
            if (CalcETX(header.GetFwd(), header.GetSrc()) > CalcETX(GetId(), header.GetSrc()) && 
                header.GetSrc() != GetId())
            {
                packet->AddHeader(fheader);
                header.SetFwd(GetId());
                packet->AddHeader(header);
                AddPacketToQueue(packet);
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
        m_phy->Send(packet);
    }
    
    return;
}

void 
LoRaMAC::SendTo (Ptr<Packet> packet, uint32_t dest)
{
    /*  send from attached node to dest node ID */
    LoRaMeshHeader header;
    
    if (m_phy)
    {
        header.SetType(DIRECTED);
        header.SetSrc(GetId());
        header.SetDest(dest);
        
        packet->AddHeader(header);
        m_phy->Send(packet);
    }
    
    return;
}

void
LoRaMAC::AddPacketToQueue (Ptr<Packet> packet)
{
    m_packet_queue.push(packet);
    return;
}

Ptr<Packet>
LoRaMAC::RemovePacketFromQueue (void)
{
    Ptr<Packet> popped = m_packet_queue.pop();
    return popped;
}

Ptr<Packet>
LoRaMAC::GetNexPacketFromQueue (void);
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
LoRaMAC::SearchLastPacketList (Ptr<Packet> packet);
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

}
}
