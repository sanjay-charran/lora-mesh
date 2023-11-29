#include "ns3/log.h"

#include "ns3/lwb-stream-request-header.h"

namespace ns3 {
namespace lora_mesh {
 
LWBStreamRequestHeader::LWBStreamRequestHeader()
{
}
 
LWBStreamRequestHeader::~LWBStreamRequestHeader()
{
}

TypeId
LWBStreamRequestHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LWBStreamRequestHeader")
        .SetParent<Header>()
        .SetGroupName("lora_mesh");
        
    return tid;
}
 
TypeId
LWBStreamRequestHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}
 
void
LWBStreamRequestHeader::SetNodeId(uint16_t node_id)
{
    m_node_id = node_id;
    return;
}
 
uint16_t 
LWBStreamRequestHeader::GetNodeId(void) const
{
    return m_node_id;
}
 
void 
LWBStreamRequestHeader::SetStreamId(uint8_t stream_id)
{
    m_stream_id = stream_id;
    return;
}
 
uint8_t 
LWBStreamRequestHeader::GetStreamId(void) const
{
    return m_stream_id;
}
 
void 
LWBStreamRequestHeader::SetIPI(uint16_t ipi)
{
    m_ipi = ipi;
    return;
}

uint16_t 
LWBStreamRequestHeader::GetIPI(void) const
{
    return m_ipi;
}

uint32_t 
LWBStreamRequestHeader::GetSerializedSize(void) const
{
    return (uint32_t)5;
}

void 
LWBStreamRequestHeader::Serialize(Buffer::Iterator start) const
{
    start.WriteU16(m_node_id);
    start.WriteU8(m_stream_id);
    start.WriteU16(m_ipi);
    
    return;
}

uint32_t 
LWBStreamRequestHeader::Deserialize(Buffer::Iterator start)
{
    start.ReadU16(m_node_id);
    start.ReadU8(m_stream_id);
    start.ReadU16(m_ipi);
    
    return GetSerializedSize();
}

void 
LWBStreamRequestHeader::Print(std::ostream &os) const
{
    os << "Node ID: " << m_node_id << std::endl;
    os << "Stream ID: " << m_stream_id << std::endl;
    os << "IPI: " << m_ipi << std::endl;
    
    return;
}

}
}
