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
#include "ns3/callback.h"
#include "ns3/double.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-net-device.h"
#include "ns3/lora-mesh-header.h"
#include "ns3/lora-mesh-routing-header.h"
#include "ns3/lora-mesh-feedback-header.h"

#include <iterator>
#include <queue>
#include <deque>

#define MAX_NUMEL_LAST_PACKETS_LIST 25

namespace ns3 {
namespace lora_mesh {
 
class LoRaPHY;
class LoRaNetDevice;
class LoRaMeshHeader;
class LoRaMeshRoutingHeader;
class LoRaMeshFeedbackHeader;

/*
 *  Structure for storing entries in a LoRaMAC's routing table
 */
struct RoutingTableEntry
{
    uint32_t    s;      /*  id's for sender and receiver    */
    uint32_t    r;
    float       etx;
    uint8_t     last;    
};

/**
 *  \brief  This class controls the custom mesh protocl MAC layer of the LoRa device.
 * 
 *  The class responsible for routing of packets in a mesh topology. Each LoRaMAC instance
 *  maintains a routing table which it will continuously update via scheduling routing updates.
 *  This class determines the most efficient path for a packet to its destination in the mesh by
 *  assessing the ETX of different paths.
 *  This class also maintains a queue of packets to be sent, and a list of the packets most 
 *  recently sent. Packets are only sent in timeslots with a random (varialbe-range) delay between
 *  them. 
 */
class LoRaMAC : public Object
{
public:
    
    LoRaMAC ();
    ~LoRaMAC ();
    
    static TypeId GetTypeId (void);
    
    /**
     *  Sets the LoRaPHY attached to this LoRaMAC
     * 
     *  \param  phy pionter to the LoRaPHY to be attached
     */
    void SetPHY (Ptr<LoRaPHY> phy);
    
    /**
     *  Gets the attached LoRaPHY to this LoRaMAC
     * 
     *  \return pointer to the attached LoRaPHY
     */
    Ptr<LoRaPHY> GetPHY (void) const;
    
    /**
     *  Sets the attached LoRaNetDevice to this LoRaMAC and adds itself as its first routing
     *  table entry
     * 
     *  \param  device  pointer to the LoRaNetDevice to be attached
     */
    void SetDevice (Ptr<LoRaNetDevice> device);
    
    /**
     *  Gets the attached LoRaNetDevice to this LoRaMAC
     * 
     *  \return pointer to the attached LoRaNetDevice
     */
    Ptr<LoRaNetDevice> GetDevice(void) const;
    
    /**
     *  Gets the Node ID of the node attached to the LoRaNetDevice attached to this LoRaMAC, 
     *  if there is no such node it returns 0.
     * 
     *  \return the Node ID of the Node attached to the LoRaNetDevice attached to this LoRaMAC
     */
    uint32_t GetId (void) const;
    
    /**
     *  Adds a routing table entry to this LoRaMAC's routing table
     * 
     *  \param  entry   the routing table entry to be added
     */
    void AddTableEntry (RoutingTableEntry entry);
    
    /**
     *  Removes a routing table entry from this LoRaMAC's routing table
     * 
     *  \param  s   the sender/source Node ID in the routing entry to be removed
     *  \param  r   the receiver/destination Node ID in the routing entry to be removed
     */
    void RemoveTableEntry (uint32_t s, uint32_t r);
    
    /**
     *  Updates the information of an already existing entry in this LoRaMAC's routing table
     * 
     *  \param  entry the entry with updated values to be used
     */
    void UpdateTableEntry (RoutingTableEntry entry);
    
    /**
     *  Checks if there is an entry in this LoRaMAC's routing table with matching source and
     *  desitnation Node IDs.
     * 
     *  \param  entry the entry with source and destination Node IDs to search for
     *
     *  \return true if the entry is found in the routing table, false otherwise 
     */
    bool EntryExists (RoutingTableEntry entry);
    
    /**
     *  Checks if an entry is an "error" entry (source and destination Node IDs are equal 
     *  with ETX being 0)
     * 
     *  \param  entry   the entry to be checked
     * 
     *  \return true if the entry is an "error" entry, false otherwise
     */
    bool IsErrEntry (RoutingTableEntry entry);
    
    /**
     *  Calculates the ETX for sending a packet from a source node to a destination node based
     *  on the information in this LoRaMAC's routing table.
     * 
     *  \param  src     source Node ID
     *  \param  dest    destination Node ID
     * 
     *  \return the computed ETX for shortest known path
     */
    float CalcETX (uint32_t src, uint32_t dest);
  
    /**
     *  Searches for a routing table entry in this LoRaMAC's routing table with a specific source 
     *  and desitnation Node ID
     * 
     *  \param  s   source/sender Node ID being searched for
     *  \param  r   receiver/destination Node ID being searched for
     * 
     *  \return the entry being searched for if it exists, otherwise an "error" entry
     */
    RoutingTableEntry TableLookup (uint32_t s, uint32_t r);
    
    /**
     *  Handles the reception of packets, based on the type of packet and if this LoRaMAC is supposed
     *  to be the recipient, and forwards and makes feedback to the packet if necessary.
     * 
     *  \param  packet  pointer to the received packet
     */
    void Receive (Ptr<Packet> packet);
    
    /**
     *  Adds the necessary packet header information and adds a packet to the packet queue for sending
     *  
     *  \param  packet  pointer to the packet to be sent
     *  \param  dest    the Node ID of the desination of the packet
     */
    void SendTo (Ptr<Packet> packet, uint32_t dest);
    
    /**
     *  Adds a packet to the packet queue for sending
     * 
     *  \param  packet  pointer to the packet to be sent
     */
    void Send (Ptr<Packet> packet);
    
    /**
     *  Generates a feedback packet for a given packet
     * 
     *  \param  packet  pointer to the packet to make a feedback packet for
     *  \param  fwd     the forwarding node of the packet the feedback packet is being made for
     *
     *  \return pointer to the generated feedback packet
     */
    Ptr<Packet> MakeFeedback (Ptr<Packet> packet, uint32_t fwd);
    
    /**
     *  Sends the next packet in the packet queue (to LoRaPHY), if not empty, and schedules the 
     *  next routing timeslot and the next instance of this function after a randomised delay.
     */
    void PacketTimeslot (void);
    
    /**
     *  Creates a new routing update packet and sends it (to LoRaPHY).
     */
    void RoutingTimeslot (void);
    
    /**
     *  Sets the minimum delay for the randomisation of the delay between packet timeslots
     * 
     *  \param  min the new minimum delay to be used for the randomisation of the delay between 
     *              packet timeslots  
     */
    void SetMinDelay(uint32_t min);
    
    /**
     *  Gets the minimum delay being used for the randomisation of the delay between packet timeslots
     * 
     *  \return the minimum delay being used for the randomisation of the delay between packet timeslots
     */
    uint32_t GetMinDelay(void) const;
    
    /**
     *  Sets the maximum delay for the randomisation of the delay between packet timeslots
     * 
     *  \param  max the new maximum delay to be used for the randomisation of the delay between 
     *              packet timeslots
     */
    void SetMaxDelay(uint32_t max);
    
    /**
     *  Gets the maximum delay being used for the randomisation of the delay between packet timeslots
     * 
     *  \return the maximum delay being used for the randomisation of the delay between packet timeslots
     */
    uint32_t GetMaxDelay(void) const;
    
private:
    
    /**
     *  Adds a packet to the packet queue for sending
     * 
     *  \param  packet      pointer to the packet to be added to the packet queue
     *  \param  isFeedback  true if the packet is a feedback packet, false otherwise
     */
    void AddPacketToQueue (Ptr<Packet> packet, bool isFeedback);
    
    /**
     *  Remove a packet from the packet queue for sending
     * 
     *  \param  pid the packet ID of the packet to be removed
     */
    void RemovePacketFromQueue (uint32_t pid);
    
    /**
     *  Checks is a packet is in the packet queue
     * 
     *  \param  pid the packet ID for the packet being searched for
     * 
     *  \return true is the packet was found in the queue, false otherwise
     */
    bool isPacketInQueue(uint32_t pid);
    
    /**
     *  Gets a pointer to the next packet in the packet queue to be sent
     * 
     *  \return pointer to the next packet in the packet queue
     */
    Ptr<Packet> GetNextPacketFromQueue (void);
    
    /**
     *  Adds a packet to the packet list of recently sent packets
     * 
     *  \param  packet a pointer to the packet to add to the packet list
     */
    void AddToLastPacketList (Ptr<Packet> packet);
    
    Ptr<LoRaPHY> m_phy;
    Ptr<LoRaNetDevice> m_device;
    
    /*  routing table containing info the end device is aware of    */
    std::deque<RoutingTableEntry> m_table;
    std::deque<RoutingTableEntry>::iterator m_cur; /*  used for deciding next routing update to send   */
    
    std::deque<Ptr<Packet>> m_packet_queue;
    std::deque<uint64_t> m_last_packets;
    uint8_t m_last_counter;
    uint32_t m_minDelay;
    uint32_t m_maxDelay;
    double m_max_packet_time;
    double m_max_routing_time;
    
    TracedCallback<Ptr<Packet>> m_rxPacketSniffer;
    TracedCallback<Ptr<Packet>> m_txPacketSniffer;
};

}
}
#endif  /*  __LORA_MAC_H__  */
