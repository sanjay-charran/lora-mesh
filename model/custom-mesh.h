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

#ifndef __CUSTOM_MESH_H__
#define __CUSTOM_MESH_H__

#include "ns3/random-variable-stream.h"
#include "ns3/mobility-model.h"
#include "ns3/vector.h"
#include "ns3/callback.h"
#include "ns3/double.h"

#include "ns3/lora-mac.h"
#include "ns3/custom-mesh-header.h"
#include "ns3/custom-mesh-routing-header.h"
#include "ns3/custom-mesh-feedback-header.h"

#include <iterator>
#include <queue>
#include <deque>

#define MAX_NUMEL_LAST_PACKETS_LIST 25

namespace ns3 {
namespace lora_mesh {

class CustomMeshHeader;
class CustomMeshRoutingHeader;
class CustomMeshFeedbackHeader;

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

class CustomMesh : public LoRaMAC
{
public:
    
    CustomMesh();
    ~CustomMesh();
    
    static TypeId GetTypeId(void);
    
    /**
     *  Adds a routing table entry to this LoRaMAC's routing table
     * 
     *  \param  entry   the routing table entry to be added
     */
    void AddTableEntry(RoutingTableEntry entry);
    
    /**
     *  Removes a routing table entry from this LoRaMAC's routing table
     * 
     *  \param  s   the sender/source Node ID in the routing entry to be removed
     *  \param  r   the receiver/destination Node ID in the routing entry to be removed
     */
    void RemoveTableEntry(uint32_t s, uint32_t r);
    
    /**
     *  Updates the information of an already existing entry in this LoRaMAC's routing table
     * 
     *  \param  entry the entry with updated values to be used
     */
    void UpdateTableEntry(RoutingTableEntry entry);
    
    /**
     *  Checks if there is an entry in this LoRaMAC's routing table with matching source and
     *  desitnation Node IDs.
     * 
     *  \param  entry the entry with source and destination Node IDs to search for
     *
     *  \return true if the entry is found in the routing table, false otherwise 
     */
    bool EntryExists(RoutingTableEntry entry);
    
    /**
     *  Checks if an entry is an "error" entry (source and destination Node IDs are equal 
     *  with ETX being 0)
     * 
     *  \param  entry   the entry to be checked
     * 
     *  \return true if the entry is an "error" entry, false otherwise
     */
    bool IsErrEntry(RoutingTableEntry entry);
    
     /**
     *  Calculates the ETX for sending a packet from a source node to a destination node based
     *  on the information in this LoRaMAC's routing table.
     * 
     *  \param  src     source Node ID
     *  \param  dest    destination Node ID
     * 
     *  \return the computed ETX for shortest known path
     */
    float CalcETX(uint32_t src, uint32_t dest);
  
    /**
     *  Searches for a routing table entry in this LoRaMAC's routing table with a specific source 
     *  and desitnation Node ID
     * 
     *  \param  s   source/sender Node ID being searched for
     *  \param  r   receiver/destination Node ID being searched for
     * 
     *  \return the entry being searched for if it exists, otherwise an "error" entry
     */
    RoutingTableEntry TableLookup(uint32_t s, uint32_t r);
    
    /**
     *  Generates a feedback packet for a given packet
     * 
     *  \param  packet  pointer to the packet to make a feedback packet for
     *  \param  fwd     the forwarding node of the packet the feedback packet is being made for
     *
     *  \return pointer to the generated feedback packet
     */
    Ptr<Packet> MakeFeedback(Ptr<Packet> packet, uint32_t fwd);
    
    /**
     *  Sends the next packet in the packet queue (to LoRaPHY), if not empty, and schedules the 
     *  next routing timeslot and the next instance of this function after a randomised delay.
     */
    void PacketTimeslot(void);
    
    /**
     *  Creates a new routing update packet and sends it (to LoRaPHY).
     */
    void RoutingTimeslot(void);
    
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
    
    /**
     *  Sets the frequency of routing updates used relative to packet timeslots (e.g., 1 for every
     *  timeslot, 2 for every other timeslot, etc.)
     * 
     *  \param  freq the value to be used to control the routing update frequency
     */
    void SetRoutingUpdateFrequency(uint32_t freq);
    
    /**
     *  Gets the value being used for controlling the routing update frequency for this LoRaMAC
     * 
     *  \return the value currently being used for controlling the routing update frequency
     */
    uint32_t GetRoutingUpdateFrequency(void) const;
    
    /**
     *  Adds the necessary packet header information and adds a packet to the packet queue for sending
     *  
     *  \param  packet  pointer to the packet to be sent
     *  \param  dest    the Node ID of the desination of the packet
     */
    void SendTo(Ptr<Packet> packet, uint32_t dest);
    
    /**
     *  Handles the reception of packets, based on the type of packet and if this LoRaMAC is supposed
     *  to be the recipient, and forwards and makes feedback to the packet if necessary.
     * 
     *  \param  packet  pointer to the received packet
     */
    void Receive(Ptr<Packet> packet);
    
    /**
     *  Adds a packet to the packet queue for sending
     * 
     *  \param  packet  pointer to the packet to be sent
     */
    void Send(Ptr<Packet> packet);
    
private:
    
    /**
     *  Adds a packet to the packet queue for sending
     * 
     *  \param  packet      pointer to the packet to be added to the packet queue
     *  \param  isFeedback  true if the packet is a feedback packet, false otherwise
     */
    void AddPacketToQueue(Ptr<Packet> packet, bool isFeedback);
    
    /**
     *  Remove a packet from the packet queue for sending
     * 
     *  \param  pid the packet ID of the packet to be removed
     */
    void RemovePacketFromQueue(uint32_t pid);
    
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
    Ptr<Packet> GetNextPacketFromQueue(void);
    
    /**
     *  Adds a packet to the packet list of recently sent packets
     * 
     *  \param  packet a pointer to the packet to add to the packet list
     */
    void AddToLastPacketList(Ptr<Packet> packet);
    
    /*  routing table containing info the end device is aware of    */
    std::deque<RoutingTableEntry> m_table;
    
    /*  Packets queued for sending  */
    std::deque<Ptr<Packet>> m_packet_queue;
    
    /*  record of last packets sent */
    std::deque<uint64_t> m_last_packets;
    
    uint8_t m_last_counter;
    
    /*  Min and Max range settings for random delay between packet timeslots    */
    uint32_t m_minDelay;
    uint32_t m_maxDelay;
    
    /*  Used to control how often routing updates are sent (e.g., 1 for every timeslot, 2 for every other, etc.)*/
    uint32_t m_routingUpdateFreq;
    uint32_t m_routingUpdateCounter;
    
    TracedCallback<Ptr<Packet>> m_rxPacketSniffer;
    TracedCallback<Ptr<Packet>> m_txPacketSniffer;
};

}
}

#endif  /*   __CUSTOM_MESH_H__   */
