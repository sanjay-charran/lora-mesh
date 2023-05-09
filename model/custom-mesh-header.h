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

#ifndef __LORA_MESH_HEADER_H__
#define __LORA_MESH_HEADER_H__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {
 
/**
 *  Enumerated type for the possible types of packets supported
 */
enum MsgType
{
    ROUTING_UPDATE = 0,     /*  broadcasting to update routing tables   */ 
    
    DIRECTED = 1,           /*  packet being sent to only a specific node   */

    FEEDBACK = 2            /*  feedback for packet reception   */
};
    
/**
 *  \brief  Main packet header used for all LoRa mesh packets
 * 
 *  This packet header is used to track information on the packets route, in terms of the 
 *  original source, destination and last forwarding node IDs. Another important parameter in 
 *  the header is the packet type which can be used identify the existence of other LoRa mesh 
 *  headers
 */
class CustomMeshHeader : public Header
{
public:
    CustomMeshHeader();
    ~CustomMeshHeader();
    
    static TypeId GetTypeId(void);
    TypeId GetInstanceTypeId(void) const;
    
    /*  virtual funcs   */
    uint32_t GetSerializedSize(void) const;
    uint32_t Deserialize(Buffer::Iterator start);
    void Serialize(Buffer::Iterator start) const;
    void Print(std::ostream &os) const;
    
    /**
     *  Sets the source ID
     * 
     *  \param  src the source ID to be set
     */
    void SetSrc(uint32_t src);
    
    /**
     *  Gets the source ID being used by the header
     * 
     *  \return the source ID being used by the header
     */
    uint32_t GetSrc(void) const;
    
    /**
     *  Sets the destination ID
     * 
     *  \param  dest    the destination ID to be set
     */
    void SetDest(uint32_t dest);
    
    /**
     *  Gets the destination ID being used by the header
     * 
     *  \return the destination ID being used by the header
     */
    uint32_t GetDest(void) const;
    
    /**
     *  Sets the type of packet
     * 
     *  \param  type    the type of packet to be set
     */
    void SetType(MsgType type);
    
    /**
     *  Gets the type of packet being used by the header
     * 
     *  \return the type of packet being used by the header
     */
    MsgType GetType(void) const;
    
    /**
     *  Sets the forwarding ID 
     * 
     *  \param  fwd the forwarding ID to be set
     */
    void SetFwd(uint32_t fwd);

    /**
     *  Gets the forwarding ID being used by the header
     * 
     *  \return the forwarding ID being used by the header
     */
    uint32_t GetFwd(void) const;
    
private:
    MsgType m_type;
    uint32_t m_src;
    uint32_t m_dest;
    uint32_t m_fwd;
};

}
}
#endif /*   __LORA_MESH_HEADER_H__  */
