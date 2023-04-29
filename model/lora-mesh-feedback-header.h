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

#ifndef __LORA_MESH_FEEDBACK_HEADER__
#define __LORA_MESH_FEEDBACK_HEADER__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {

/**
 *  \brief  Packet header to be attached to LoRa mesh feedback packets
 *  
 *  This header is used to add the packet ID of the packet for which the feedback packet was
 *  made which is not included in the general LoRa mesh header.
 */
class LoRaMeshFeedbackHeader : public Header
{
public:
    
    LoRaMeshFeedbackHeader();
    ~LoRaMeshFeedbackHeader();
    
    static TypeId GetTypeId(void);
    TypeId GetInstanceTypeId(void) const;
    
    /*  virtual funcs   */
    uint32_t GetSerializedSize(void) const;
    uint32_t Deserialize(Buffer::Iterator start);
    void Serialize(Buffer::Iterator start) const;
    void Print(std::ostream &os) const;
    
    /**
     * Sets the packet ID
     * 
     * \param   packet_id   the packet ID to be set
     */
    void SetPacketId (uint64_t packet_id);
    
    /**
     *  Gets the packet ID being used by the header
     * 
     *  \return the packet ID being used by the header
     */
    uint64_t GetPacketId (void) const;
    
private:
    uint64_t m_packetid;
};
    
}
}

#endif /*   __LORA_MESH_FEEDBACK_HEADER__   */
