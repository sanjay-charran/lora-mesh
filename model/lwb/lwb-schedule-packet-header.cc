#include "ns3/log.h"

#include "ns3/lwb-schedule-packet-header.h"

namespace ns3 {
namespace lora_mesh {
 
LWBSchedulePacketHeader::LWBSchedulePacketHeader()
{
}

LWBSchedulePacketHeader::~LWBSchedulePacketHeader()
{
}

TypeId
LWBSchedulePacketHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LWBSchedulePacketHeader")
        .SetParent<Header>()
        .SetGroupName("lora_mesh");
        
    return tid;
}

TypeId
LWBSchedulePacketHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void
LWBSchedulePacketHeader::SetTime(uint32_t time)
{
    m_time = time;
    return;
}

uint32_t
LWBSchedulePacketHeader::GetTime(void) const
{
    return m_time;
}

void
LWBSchedulePacketHeader::SetPeriod(uint16_t period)
{
    m_period = period;
    return;
}

uint16_t
LWBSchedulePacketHeader::GetPeriod(void) const
{
    return m_period;
}

void
LWBSchedulePacketHeader::SetNSlots(uint16_t n_slots)
{
    m_n_slots = n_slots;
    return;
}

uint16_t
LWBSchedulePacketHeader::GetNSlots(void) const
{
    return m_n_slots;
}

void 
LWBSchedulePacketHeader::SetSlots(uint16_t slot[LWB_MAX_DATA_SLOTS])
{
    unsigned int i;
    
    for (i = 0;i < LWB_MAX_DATA_SLOTS;i++)
    {
        m_slot[i] = slot[i];
    }

    return;
}

void 
LWBSchedulePacketHeader::GetSlots(uint16_t slot[LWB_MAX_DATA_SLOTS]) const
{
    unsigned int i;
    
    for (i = 0;i < LWB_MAX_DATA_SLOTS;i++)
    {
        slot[i] = m_slot[i];
    }
    
    return;
}

uint32_t
LWBSchedulePacketHeader::GetSerializedSize(void) const
{
    return (uint32_t)(8 + (2 * m_n_slots));
}

void
LWBSchedulePacketHeader::Serialize(Buffer::Iterator start) const
{
    unsigned int i;
    
    start.WriteU32(m_time);
    start.WriteU16(m_period);
    start.WriteU16(m_n_slots);
    
    for (i = 0;i < m_n_slots;i++)
    {
        start.WriteU16(m_slot[i]);
    }
    
    return;
}

uint32_t
LWBSchedulePacketHeader::Deserialize(Buffer::Iterator start)
{
    unsigned int i;
    
    m_time = start.ReadU32();
    m_period = start.ReadU16();
    m_n_slots = start.ReadU16();
    
    for (i = 0;i < m_n_slots;i++)
    {
        m_slot[i] = start.ReadU16();
    }
    
    return GetSerializedSize();
}

void
LWBSchedulePacketHeader::Print(std::ostream &os) const
{
    unsigned int i;
    
    os << "Time: " << m_time << std::endl;
    os << "Period: " << m_period << std::endl;
    os << "Number of Slots: " << m_n_slots << std::endl;
    
    for (i = 0;i < m_n_slots;i++)
    {
        os << "\tSlot #" << i + 1 << ": " << m_slot[i] << std::endl;
    }
    
    return;
}

}
}
