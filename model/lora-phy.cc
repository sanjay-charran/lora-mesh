/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"

#include "ns3/lora-phy.h"

namespace ns3 {
namespace lora_mesh {

TypeId LoRaPHY::GetTypeId (void)
{
    static tid = TypeId ("ns3::LoRaPHY")
        .SetParent<Object> ()
        .SetGroupName ("lora_mesh");
        
    return tid;
}
    
LoRaPHY::LoRaPHY ()
{
}

LoRaPHY::~LoRaPHY ()
{
}

void LoRaPHY::SetID (uint16_t id)
{
    m_id = id;
    return;
}

uint16_t LoRaPHY::GetID (void) const
{
    return m_id;
}

void LoRaPHY::SetNetDevice (Ptr<LoRaNetDevice> lnd)
{
    m_device = lnd;
    return;
}

Ptr<LoRaNetDevice> LoRaPHY::GetNetDevice (void) const
{
    return m_device;
}

void SetChannel (Ptr<LoRaMeshChannel> c)
{
    m_channel = c;
    return;
}

Ptr<LoRaMeshChannel> LoRaPHY::GetChannel (void) const
{
    return m_channel;
}

void LoRaPHY::AddTableEntry (RoutingTableEntry entry)
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

RoutingTableEntry RemoveTableEntry (utin64_t n)
{
    
}

}
}
