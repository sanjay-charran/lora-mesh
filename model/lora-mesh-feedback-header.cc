/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
