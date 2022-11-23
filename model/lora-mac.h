/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __LORA_MAC_H__
#define __LORA_MAC_H__

#include "ns3/object.h"
#include "ns3/ptr.h"

#include <list>
#include <iterator>

namespace ns3 {
namespace lora_mesh {
 
struct RoutingTableEntry
{
    uint16_t    s;      /*  id's for sender and receiver    */
    uint16_t    r;
    float       etx;
    uint8_t     last;    
};
    
class LoRaMAC : pulic Object
{
public:
    
    /*  constructor and destructor  */
    LoRaMAC ();
    ~LoRaMAC ();
    
    /*  TypeId  */
    static TypeId GetTypeId (void);
    
    /*  setter and getter for unique id    */
    void SetID (uint16_t id);
    uint16_t GetID (void) const;
    
    /*  add, update, and remove entry for routing table  */
    void AddTableEntry (RoutingTableEntry entry);
    void RemoveTableEntry (utin16_t s, uint16_t r);
    void UpdateTableEntry (RoutingTableEntry entry);
    
    /*  routing table lookup    */
    RoutingTableEntry TableLookup (uint16_t s, uint16_t r) const;
    RoutingTableEntry TableLookup (uint64_t n) const;
    
    //add in forwarding logic and feedback
    
private:
    
    /*  unique id for */
    uint16_t m_id;
    
    /*  routing table containing info the end device is aware of    */
    list<RoutingTableEntry> m_table;
};

}
}
#endif  /*  __LORA_MAC_H__  */
