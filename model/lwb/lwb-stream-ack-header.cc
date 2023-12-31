#include "ns3/log.h"

#include "ns3/lwb-stream-ack-header.h"

namespace ns3 {
namespace lora_mesh {
 
LWBStreamACKHeader::LWBStreamACKHeader()
{
}
 
LWBStreamACKHeader::~LWBStreamACKHeader()
{
}

TypeId
LWBStreamACKHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LWBStreamACKHeader")
        .SetParent<Header>()
        .SetGroupName("lora_mesh");
        
    return tid;
}
 
TypeId
LWBStreamACKHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}
 
void
LWBStreamACKHeader::SetNodeId(uint16_t node_id)
{
    m_node_id = node_id;
    return;
}
 
uint16_t 
LWBStreamACKHeader::GetNodeId(void) const
{
    return m_node_id;
}
 
void 
LWBStreamACKHeader::SetStreamId(uint8_t stream_id)
{
    m_stream_id = stream_id;
    return;
}
 
uint8_t 
LWBStreamACKHeader::GetStreamId(void) const
{
    return m_stream_id;
}

uint32_t 
LWBStreamACKHeader::GetSerializedSize(void) const
{
    return (uint32_t)3;
}

void 
LWBStreamACKHeader::Serialize(Buffer::Iterator start) const
{
    start.WriteU16(m_node_id);
    start.WriteU8(m_stream_id);
    
    return;
}

uint32_t 
LWBStreamACKHeader::Deserialize(Buffer::Iterator start)
{
    m_node_id = start.ReadU16();
    m_stream_id = start.ReadU8();
    
    return GetSerializedSize();
}

void 
LWBStreamACKHeader::Print(std::ostream &os) const
{
    os << "Node ID: " << m_node_id << std::endl;
    os << "Stream ID: " << m_stream_id << std::endl;
    
    return;
}

}
}
