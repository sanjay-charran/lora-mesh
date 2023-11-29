#ifndef __LWB_SCEDULE_PACKET_HEADER_H__
#define __LWB_SCEDULE_PACKET_HEADER_H__

#include "ns3/header.h"

#ifndef LWB_MAX_DATA_SLOTS
#define LWB_MAX_DATA_SLOTS  20
#endif  /*  LWB_MAX_DATA_SLOTS  */

namespace ns3 {
namespace lora_mesh {
    
class LWBSchedulePacketHeader : public header
{
public:
    LWBSchedulePacketHeader();
    ~LWBSchedulePacketHeader();
    
    static TypeId GetTypeId(void);
    TypeId GetInstanceTypeId(void) const;
    
    /*  virtual funcs   */
    uint32_t GetSerializedSize(void) const;
    uint32_t Deserialize(Buffer::Iterator start);
    void Serialize(Buffer::Iterator start) const;
    void Print(std::ostream &os) const;
    
    void SetTime(uint32_t time);
    uint32_t GetTime(void) const;
    
    void SetPeriod(uint16_t period);
    uint16_t GetPeriod(void) const;
    
    void SetNSlots(uint16_t n_slots);
    uint16_t GetNSlots(void) const;
    
    void SetSlots(uint16_t slot[LWB_MAX_DATA_SLOTS]);
    void GetSlots(uint16_t slot[LWB_MAX_DATA_SLOTS]) const;
    
private:
    uint32_t m_time;
    uint16_t m_period;
    uint16_t m_n_slots;
    uint16_t m_slot[LWB_MAX_DATA_SLOTS];
};

}
}

#endif /*   __LWB_SCEDULE_PACKET_HEADER_H__    */
