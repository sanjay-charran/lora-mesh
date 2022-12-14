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
    list<RoutingTableEntry>::iterator it = m_table.begin();
    
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
    list<RoutingTableEntry>::iterator it = m_table.begin();
    
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
    list<RoutingTableEntry>::iterator it = m_table.begin();
    
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
    list<RoutingTableEntry>::iterator it = m_table.begin();
    
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
    list<RoutingTableEntry>::iterator it = m_table.begin();
    
    if (m_table.size() <= n)
    {
        /*  number not in valid range */
        return err;
    }
    
    advance (it, n);
    return (*it);
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

}
}
