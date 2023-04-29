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

#include "ns3/lora-mesh-routing-header.h"

namespace ns3 {
namespace lora_mesh {
 
LoRaMeshRoutingHeader::LoRaMeshRoutingHeader()    
{    
    m_etx = 0;
    m_last = 0;
}
 
LoRaMeshRoutingHeader::~LoRaMeshRoutingHeader()
{
}
 
TypeId
LoRaMeshRoutingHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LoRaMeshRoutingHeader")
        .SetParent<Header>()
        .SetGroupName("lora_mesh");
        
    return tid;;
}

TypeId
LoRaMeshRoutingHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
LoRaMeshRoutingHeader::SetETX(float etx)
{
    m_etx = etx;
    return;
}

float
LoRaMeshRoutingHeader::GetETX(void) const
{
    return m_etx;
}

void
LoRaMeshRoutingHeader::SetLast(uint8_t last)
{
    m_last = last;
    return;
}

uint8_t
LoRaMeshRoutingHeader::GetLast(void) const
{
    return m_last;
}

uint32_t
LoRaMeshRoutingHeader::GetSerializedSize(void) const
{
    /*  should be 4(etx) + 1(last) = 5 */
    return (uint32_t)(sizeof(float) + 1);
}

void
LoRaMeshRoutingHeader::Serialize(Buffer::Iterator start) const
{
    start.Write((uint8_t *)&m_etx, sizeof(float));
    start.WriteU8(m_last);
    
    return;
}

uint32_t
LoRaMeshRoutingHeader::Deserialize(Buffer::Iterator start)
{
    start.Read((uint8_t *)&m_etx, sizeof(float));
    m_last = start.ReadU8();
    
    return GetSerializedSize();
}

void
LoRaMeshRoutingHeader::Print(std::ostream &os) const
{
    os << "ETX: " << m_etx << std::endl;
    os << "Last Counter: " << m_last << std::endl;
    
    return;
}

}
}
