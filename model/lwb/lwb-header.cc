#include "ns3/log.h"

#include "ns3/lwb-header.h"

namespace ns3 {
namespace lora_mesh {
 
LWBHeader::LWBHeader()
{
}
    
LWBHeader::~LWBHeader()
{
}

TypeId
LWBHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LWBHeader")
        .SetParent<Header>()
        .SetGroupName("lora_mesh");
        
    return tid;
}

TypeId
LWBHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
LWBHeader::SetRecipientId(uint16_t rid)
{
    m_recipient_id = rid;
    return;
}

uint16_t
LWBHeader::GetRecipientId(void) const
{
    return m_recipient_id;
}

void
LWBHeader::SetStreamId(uint8_t sid)
{
    m_stream_id = sid;
    return;
}

uint8_t
LWBHeader::GetStreamid(void) const
{
    return m_stream_id;
}

uint32_t
LWBHeader::GetSerializedSize(void) const
{
    return (uint32_t)3;
}

void
LWBHeader::Serialize(Buffer::Iterator start) const
{
    start.WriteU16(m_recipient_id);
    start.WriteU8(m_stream_id);
    
    return;
}

uint32_t
LWBHeader::Deserialize(Buffer::Iterator start)
{
    m_recipient_id = start.ReadU16();
    m_stream_id = start.ReadU8();
    
    return GetSerializedSize();
}

void
LWBHeader::Print(std::ostream &os) const
{
    os << "Recipient ID: " << m_recipient_id << std::endl;
    os << "Stream ID: " << m_stream_id << std::endl;
    
    return;
}

}
}
