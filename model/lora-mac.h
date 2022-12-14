/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __LORA_MAC_H__
#define __LORA_MAC_H__

#include "ns3/object.h"
#include "ns3/ptr.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-net-device.h"
#include "ns3/lora-mesh-header.h"

#include <list>
#include <iterator>

namespace ns3 {
namespace lora_mesh {
 
struct RoutingTableEntry
{
    uint32_t    s;      /*  id's for sender and receiver    */
    uint32_t    r;
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
    
    void SetPHY (Ptr<LoRaPHY> phy);
    Ptr<LoRaPHY> GetPHY (void) const;
    
    void SetDevice (Ptr<LoRaNetDevice> device);
    Ptr<LoRaNetDevice> GetDevice(void) const;
    
    uint32_t GetId (void) const;
    
    /*  add, update, and remove entry for routing table  */
    void AddTableEntry (RoutingTableEntry entry);
    void RemoveTableEntry (uint32_t s, uint32_t r);
    void UpdateTableEntry (RoutingTableEntry entry);
    bool IsErrEntry (RoutingTableEntry entry);
    
    /*  routing table lookup    */
    RoutingTableEntry TableLookup (uint32_t s, uint32_t r) const;
    RoutingTableEntry TableLookup (uint64_t n) const;
    
    void ForwardPacket (Ptr<Packet> packet);//
    void Broadcast (Ptr<Packet> packet);
    void SendTo (Ptr<Packet> packet, uint32_t dest);
    //  need to add scheduling for timeslots at this layer
private:
    
    Ptr<LoRaPHY> m_phy;
    Ptr<LoRaNetDevice> m_device;
    
    /*  routing table containing info the end device is aware of    */
    list<RoutingTableEntry> m_table;
};

}
}
#endif  /*  __LORA_MAC_H__  */
