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
LWBHeader::SetPacketType(lwb_packet_type type)
{
    m_type = type;
    return;
}

lwb_packet_type
LWBHeader::GetPacketType(void) const
{
    return m_type;
}

void 
LWBHeader::Serialize(Buffer::Iterator start) const
{
    Start.WriteU8((uint8_t)m_type);
    
    return;
}

uint32_t
LWBHeader::Deserialize(Buffer::Iterator start)
{
    m_type = start.ReadU8();
    
    return GetSerializedSize();
}

uint32_t
LWBHeader::GetSerializedSize(void) const
{
    return 1;
}

void 
LWBHeader::Print(std::ostream &os) const
{
    os << "LWB Packet Type: ";
    
    switch (m_type)
    {
        case SCHEDULE:          os << "Schedule";
                                break;
                                
        case DATA:              os << "Data";
                                break;
                                
        case STREAM_REQUEST:    os << "Stream Request";
                                break;
                                
        case STREAM_ACK:        os << "Stream ACK";
                                break;
                                
        case DATA_ACK:          os << "Data ACK";
                                break;
    }
    
    os << std::endl;
}

}
}
