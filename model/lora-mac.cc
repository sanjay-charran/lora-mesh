/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"

#include "ns3/lora-mac.h"

namespace ns3 {
namespace lora_mesh {
 
TypeId LoRaMAC::GetTypeId (void)
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

void LoRaMAC::SetID (uint16_t id)
{
    m_id = id;
    return;
}

uint16_t LoRaMAC::GetID (void) const
{
    return m_id;
}

void LoRaMAC::AddTableEntry (RoutingTableEntry entry)
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

void LoRaMAC::RemoveTableEntry (utin16_t s, uint16_t r)
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

void LoRaMAC::UpdateTableEntry (RoutingTableEntry entry)
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

RoutingTableEntry LoRaMAC::TableLookup (uint16_t s, uint16_t r) const
{
    RoutingTableEntry err = {m_id, m_id, 1, 0}; /*  an error return since it ought to be 0 etx for node to transmit to itself   */
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

RoutingTableEntry LoRaMAC::TableLookup (uint64_t n) const
{
    RoutingTableEntry err = {m_id, m_id, 1, 0}; /*  an error return since it ought to be 0 etx for node to transmit to itself   */
    list<RoutingTableEntry>::iterator it = m_table.begin();
    
    if (m_table.size() <= n)
    {
        /*  number not in valid range */
        return err;
    }
    
    advance (it, n);
    return (*it);
}

}
}
