#include "ns3/log.h"

#include "ns3/lwb-data-packet-header.h"

namespace ns3 {
namespace lora_mesh {
 
LWBDataPacketHeader::LWBDataPacketHeader()
{
}
    
LWBDataPacketHeader::~LWBDataPacketHeader()
{
}

TypeId
LWBDataPacketHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LWBDataPacketHeader")
        .SetParent<Header>()
        .SetGroupName("lora_mesh");
        
    return tid;
}

TypeId
LWBDataPacketHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
LWBDataPacketHeader::SetRecipientId(uint16_t rid)
{
    m_recipient_id = rid;
    return;
}

uint16_t
LWBDataPacketHeader::GetRecipientId(void) const
{
    return m_recipient_id;
}

void
LWBDataPacketHeader::SetStreamId(uint8_t sid)
{
    m_stream_id = sid;
    return;
}

uint8_t
LWBDataPacketHeader::GetStreamid(void) const
{
    return m_stream_id;
}

uint32_t
LWBDataPacketHeader::GetSerializedSize(void) const
{
    return (uint32_t)3;
}

void
LWBDataPacketHeader::Serialize(Buffer::Iterator start) const
{
    start.WriteU16(m_recipient_id);
    start.WriteU8(m_stream_id);
    
    return;
}

uint32_t
LWBDataPacketHeader::Deserialize(Buffer::Iterator start)
{
    m_recipient_id = start.ReadU16();
    m_stream_id = start.ReadU8();
    
    return GetSerializedSize();
}

void
LWBDataPacketHeader::Print(std::ostream &os) const
{
    os << "Recipient ID: " << m_recipient_id << std::endl;
    os << "Stream ID: " << m_stream_id << std::endl;
    
    return;
}

}
}
