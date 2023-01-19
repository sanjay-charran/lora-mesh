/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/lora-mesh-routing-header.h"

namespace ns3 {
namespace lora_mesh {
 
LoRaMeshRoutingHeader::LoRaMeshRoutingHeader()    
{    
    m_etx = 0;
    m_last = 0;
}
 
LoRaMeshRoutingHeader::~LoRaMeshRoutingHeader ()
{
}
 
TypeId
LoRaMeshRoutingHeader::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::LoRaMeshRoutingHeader")
        .SetParent<Header> ()
        .SetGroupName ("lora_mesh");
        
    return tid;;
}

TypeId
LoRaMeshRoutingHeader::GetInstanceTypeId (void) const
{
    return GetTypeId();
}

void
LoRaMeshRoutingHeader::SetETX (float etx)
{
    m_etx = etx;
    return;
}

float
LoRaMeshRoutingHeader::GetETX (void) const
{
    return m_etx;
}

void
LoRaMeshRoutingHeader::SetLast (uint8_t last)
{
    m_last = last;
    return;
}

uint8_t
LoRaMeshRoutingHeader::GetLast (void) const
{
    return m_last;
}

uint32_t
LoRaMeshRoutingHeader::GetSerializedSize (void) const
{
    /*  should be 4(etx) + 1(last) = 5 */
    return (uint32_t)(sizeof(float) + 1);
}

void
LoRaMeshRoutingHeader::Serialize (Buffer::Iterator start) const
{
    start.Write ((uint8_t *)&m_etx, 4);
    start.WriteU8 (m_last);
    
    return;
}

uint32_t
LoRaMeshRoutingHeader::Deserialize (Buffer::Iterator start)
{
    start.Read ((uint8_t *)&m_etx, sizeof(float));
    m_last = start.ReadU8 ();
    
    return GetSerializedSize ();
}

void
LoRaMeshRoutingHeader::Print (std::ostream &os) const
{
    os << "ETX: " << m_etx << std::endl;
    os << "Last Counter: " << m_last << std::endl;
    
    return;
}

}
}
