/*
 * Copyright (c) 2015, Swiss Federal Institute of Technology (ETH Zurich).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author:  Federico Ferrari
 *          Reto Da Forno
 *          Marco Zimmerling
 */

/*  Modified by: Sanjay Charran <sanjaycharran@gmail.com>   */

#include "ns3/log.h"

#include "ns3/glossy-header.h"

namespace ns3 {
namespace lora_mesh {
 
GlossyHeader::GlossyHeader()
{
}
 
GlossyHeader::~GlossyHeader()
{
}
 
TypeId
GlossyHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::GlossyHeader")
        .SetParent<Header>()
        .SetGroupName("lora_mesh");
        
    return tid;
}

TypeId
GlossyHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
GlossyHeader::SetPacketLen(uint8_t packet_len)
{
    m_packet_len = packet_len;
    return;
}

uint8_t
GlossyHeader::GetPacketLen(void) const
{
    return m_packet_len;
}

void
GlossyHeader::SetInitiatorId(uint16_t initiator_id)
{
    m_initiator_id = initiator_id;
    return;
}

uint16_t
GlossyHeader::GetInitiatorId(void) const
{
    return m_initiator_id;
}

void 
GlossyHeader::SetPktType(uint8_t pkt_type)
{
    m_pkt_type = pkt_type;
    return;
}

uint8_t
GlossyHeader::GetPktType(void) const
{
    return m_pkt_type;
}

void
GlossyHeader::SetRelayCnt(uint8_t relay_cnt)
{
    m_relay_cnt = relay_cnt;
    return;
}

uint8_t
GlossyHeader::GetRelayCnt(void) const
{
    return m_relay_cnt;
}

uint32_t
GlossyHeader::GetSerializedSize(void) const
{
    return (uint32_t)5;
}

void
GlossyHeader::Serialize(Buffer::Iterator start) const
{
    start.WriteU16(m_initiator_id);
    start.WriteU8(m_pkt_type);
    start.WriteU8(m_relay_cnt);
    
    return;
}

uint32_t
GlossyHeader::Deserialize(Buffer::Iterator start)
{
    m_initiator_id = start.ReadU16();
    m_pkt_type = start.ReadU8();
    m_relay_cnt = start.ReadU8();
    
    return GetSerializedSize();
}

void
GlossyHeader::Print(std::ostream &os) const
{
    os << "Initiator ID: " << m_initiator_id << std::endl;
    os << "Packet Type ID: " << m_pkt_type << std::endl;
    os << "Relay Count: " << m_relay_cnt << std::endl;
    
    return;
}

}
}
