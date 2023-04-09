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

#include "ns3/lora-mesh-feedback-header.h"

namespace ns3 {
namespace lora_mesh {

LoRaMeshFeedbackHeader::LoRaMeshFeedbackHeader()
{
    m_packetid = 0;
}
    
LoRaMeshFeedbackHeader::~LoRaMeshFeedbackHeader()
{
}
    
TypeId
LoRaMeshFeedbackHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LoRaMeshFeedbackHeader")
        .SetParent<Header>()
        .SetGroupName("lora_mesh");
        
    return tid;
}

TypeId 
LoRaMeshFeedbackHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}
    
void
LoRaMeshFeedbackHeader::SetPacketId(uint64_t packet_id)
{
    m_packetid = packet_id;
    return;
}
    
uint64_t
LoRaMeshFeedbackHeader::GetPacketId(void) const
{
    return m_packetid;
}
    
uint32_t
LoRaMeshFeedbackHeader::GetSerializedSize(void) const
{
    return 8;
}
 
void
LoRaMeshFeedbackHeader::Serialize(Buffer::Iterator start) const
{
    start.WriteU64(m_packetid);
    return;
}
 
uint32_t
LoRaMeshFeedbackHeader::Deserialize(Buffer::Iterator start)
{
    m_packetid = start.ReadU64();
    
    return GetSerializedSize();
}

void 
LoRaMeshFeedbackHeader::Print(std::ostream &os) const
{
    os << "Feedback for Packet: " << m_packetid << std::endl;
    
    return;
}
 
}
}
