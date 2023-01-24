/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __LORA_MAC_H__
#define __LORA_MAC_H__

#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/random-variable-stream.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model.h"
#include "ns3/vector.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-net-device.h"
#include "ns3/lora-mesh-header.h"
#include "ns3/lora-mesh-routing-header.h"
#include "ns3/lora-mesh-feedback-header.h"

#include <iterator>
#include <queue>
#include <deque>

#define MAX_NUMEL_LAST_PACKETS_LIST 25

#define MAX_PACKET_MSG_LENGTH_BYTES 50

#define MIN_RAND_UINT32  1
#define MAX_RAND_UINT32  10

namespace ns3 {
namespace lora_mesh {
 
class LoRaPHY;
class LoRaNetDevice;
class LoRaMeshHeader;
class LoRaMeshRoutingHeader;
class LoRaMeshFeedbackHeader;

struct RoutingTableEntry
{
    uint32_t    s;      /*  id's for sender and receiver    */
    uint32_t    r;
    float       etx;
    uint8_t     last;    
};

class LoRaMAC : public Object
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
    bool EntryExists (RoutingTableEntry entry);
    bool IsErrEntry (RoutingTableEntry entry);
    
    float CalcETX (uint32_t src, uint32_t dest);
  
    /*  routing table lookup    */
    RoutingTableEntry TableLookup (uint32_t s, uint32_t r);
    RoutingTableEntry TableLookup (uint64_t n);
    
    void Receive (Ptr<Packet> packet);
    
    void Broadcast (Ptr<Packet> packet);
    void SendTo (Ptr<Packet> packet, uint32_t dest);
    void Send (Ptr<Packet> packet); //assumes packet already sorted out header data
    
    Ptr<Packet> MakeFeedback (Ptr<Packet> packet, uint32_t fwd);
    
    void PacketTimeslot (void);
    void RoutingTimeslot (void);
    
private:
    
    /*  packet queue funcs  */
    void AddPacketToQueue (Ptr<Packet> packet, bool isFeedback);
    void RemovePacketFromQueue (void);
    Ptr<Packet> GetNextPacketFromQueue (void);
    
    /*  handled packet list */
    void AddToLastPacketList (Ptr<Packet> packet);
    bool SearchLastPacketList (Ptr<Packet> packet);
    //bool SearchLastPacketList (uint64_t uid);//
    
    //float CalcETX (uint32_t src, uint32_t dest, uint32_t last);//improve later
    
    Ptr<LoRaPHY> m_phy;
    Ptr<LoRaNetDevice> m_device;
    
    /*  routing table containing info the end device is aware of    */
    std::deque<RoutingTableEntry> m_table;
    std::deque<RoutingTableEntry>::iterator m_cur; /*  used for deciding next routing update to send   */
    
    std::deque<Ptr<Packet>> m_packet_queue;
    std::deque<uint64_t> m_last_packets;
    uint8_t m_last_counter;
    
    double m_max_packet_time;
    double m_max_routing_time;
};

}
}
#endif  /*  __LORA_MAC_H__  */
