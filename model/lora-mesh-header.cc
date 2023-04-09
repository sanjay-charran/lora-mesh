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

#include "ns3/log.h"

#include "ns3/lora-mesh-header.h"


namespace ns3 {
namespace lora_mesh {
    
LoRaMeshHeader::LoRaMeshHeader ()
{
    /*  init to irrational value if received by another node    */
    m_src = 0;
    m_dest = 0;
    m_fwd = 0;
    m_type = DIRECTED;
}
    
LoRaMeshHeader::~LoRaMeshHeader ()
{
}
 
TypeId
LoRaMeshHeader::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::LoRaMeshHeader")
        .SetParent<Header> ()
        .SetGroupName ("lora_mesh");
        
    return tid;
}

TypeId
LoRaMeshHeader::GetInstanceTypeId (void) const
{
    return GetTypeId();
}

void
LoRaMeshHeader::SetSrc (uint32_t src)
{
    m_src = src;
    return;
}

uint32_t
LoRaMeshHeader::GetSrc (void) const
{
    return m_src;
}
 
void
LoRaMeshHeader::SetDest (uint32_t dest)
{
    m_dest = dest;
    return;
}

uint32_t
LoRaMeshHeader::GetDest (void) const
{
    return m_dest;
}
 
void 
LoRaMeshHeader::SetType (MsgType type)
{
    m_type = type;
    return;
}

MsgType
LoRaMeshHeader::GetType (void) const
{
    return m_type;
}

void
LoRaMeshHeader::SetFwd (uint32_t fwd)
{
    m_fwd = fwd;
    return;
}

uint32_t
LoRaMeshHeader::GetFwd (void) const
{
    return m_fwd;
}

uint32_t
LoRaMeshHeader::GetSerializedSize (void) const
{
    /*  4(src) + 4(dest) + 4(fwd) + 1(type)  = 13 */
    /*  type is 1 byte since only 4 options in enum  */
    return (uint32_t)13;
}

void
LoRaMeshHeader::Serialize (Buffer::Iterator start) const
{
    start.WriteU8 ((uint8_t)m_type);
    start.WriteU32 (m_src);
    start.WriteU32 (m_dest);
    start.WriteU32 (m_fwd);
    
    return;
}

uint32_t
LoRaMeshHeader::Deserialize (Buffer::Iterator start)
{
    m_type = (MsgType)(start.ReadU8());
    m_src = start.ReadU32 ();
    m_dest = start.ReadU32 ();
    m_fwd = start.ReadU32 ();
    
    return GetSerializedSize ();
}

void 
LoRaMeshHeader::Print (std::ostream &os) const
{
    os << "Message Type: " << ((m_type == ROUTING_UPDATE)?"ROUTING_UPDATE":(m_type == DIRECTED?"DIRECTED":"FEEDBACK")) << std::endl;
    os << "Source ID: " << m_src << std::endl;
    os << "Destination ID: " << m_dest << std::endl;
    os << "Last Forwarder: " << m_fwd << std::endl;
    
    return;
}

}
}
