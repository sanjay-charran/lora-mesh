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

#ifndef __LORA_MESH_ROUTING_HEADER_H__
#define __LORA_MESH_ROUTING_HEADER_H__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {

/**
 *  \brief  Packet header for LoRa mesh routing updates
 * 
 *  This header is used to contain the additional information required for routing table entries
 *  by LoRa mesh nodes that is not already contained in the the general LoRa mesh header. These two
 *  missing parameters are the expected transmission count (ETX) and the counter value the last time 
 *  this entry was sent by the node
 */
class CustomMeshRoutingHeader : public Header
{
public:

    CustomMeshRoutingHeader();
    ~CustomMeshRoutingHeader();
    
    static TypeId GetTypeId(void);
    TypeId GetInstanceTypeId(void) const;
    
    /*  virtual funcs   */
    uint32_t GetSerializedSize(void) const;
    uint32_t Deserialize(Buffer::Iterator start);
    void Serialize(Buffer::Iterator start) const;
    void Print(std::ostream &os) const;
    
    /**
     *  Sets the ETX of the routing update
     * 
     *  \param  etx the ETX to be set
     */
    void SetETX(float etx);
    
    /**
     *  Gets the ETX used by the header
     * 
     *  \return the ETX used by the header
     */
    float GetETX(void) const;
    
    /**
     *  Sets the value of the counter the last time this routing entry was sent
     * 
     *  \param last the value of the counter the last time this routing entry was sent
     */
    void SetLast(uint8_t last);
    
    /**
     *  Gets the of the counter the last time this routing entry was sent being used by the header
     * 
     *  \return the of the counter the last time this routing entry was sent being used by the header
     */
    uint8_t GetLast(void) const;
    
private:
    float m_etx;
    uint8_t m_last;
};

}
}

#endif  /*  __LORA_MESH_ROUTING_HEADER_H__  */
