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

#ifndef __LORA_MAC_H__
#define __LORA_MAC_H__

#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/vector.h"
#include "ns3/packet.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-net-device.h"


namespace ns3 {
namespace lora_mesh {
 
class LoRaPHY;
class LoRaNetDevice;

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
    
    LoRaMAC();
    ~LoRaMAC();
    
    static TypeId GetTypeId(void);
    
    /**
     *  Sets the LoRaPHY attached to this LoRaMAC
     * 
     *  \param  phy pionter to the LoRaPHY to be attached
     */
    void SetPHY(Ptr<LoRaPHY> phy);
    
    /**
     *  Gets the attached LoRaPHY to this LoRaMAC
     * 
     *  \return pointer to the attached LoRaPHY
     */
    Ptr<LoRaPHY> GetPHY(void) const;
    
    /**
     *  Sets the attached LoRaNetDevice to this LoRaMAC and adds itself as its first routing
     *  table entry
     * 
     *  \param  device  pointer to the LoRaNetDevice to be attached
     */
    void SetDevice(Ptr<LoRaNetDevice> device);
    
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
    uint32_t GetId(void) const;
    
    /**
     *  Receives packet (intended to be overwritten by derived class to suit protocol being used)
     * 
     *  \param  packet  pointer to the received packet
     */
    virtual void Receive(Ptr<Packet> packet);
    
    /**
     *  Sends packet through attached LoRaPHY layer via LoRaChannel attached to the PHY 
     * 
     *  \param  packet  pointer to the packet to be sent
     */
    virtual void Send(Ptr<Packet> packet);
    
protected:
    
    Ptr<LoRaPHY>        m_phy;
    Ptr<LoRaNetDevice>  m_device;
};

}
}

#endif  /*  __LORA_MAC_H__  */
