#ifndef __LWB_SCEDULE_PACKET_HEADER_H__
#define __LWB_SCEDULE_PACKET_HEADER_H__

#include "ns3/header.h"

#ifndef LWB_MAX_DATA_SLOTS
#define LWB_MAX_DATA_SLOTS  20
#endif  /*  LWB_MAX_DATA_SLOTS  */

#define LWB_CONF_SCHED_PERIOD_IDLE  10
#define LWB_SCHED_DACK_SLOT         0x2000
#define LWB_SCHED_CONT_SLOT         0x4000
#define LWB_SCHED_SACK_SLOT         0x8000


/**
 * @brief marks the schedule as the 1st schedule
 */
#define LWB_SCHED_SET_AS_1ST(s)       ((s)->period |= 0x8000)
/**
 * @brief marks the schedule s as the 2nd schedule 
 */
#define LWB_SCHED_SET_AS_2ND(s)       ((s)->period &= ~0x8000)
/**
 * @brief checks whether schedule is the 1st schedule
 */
#define LWB_SCHED_IS_1ST(s)           (((s)->period & 0x8000) > 0)
/**
 * @brief checks whether schedule is the 2nd schedule
 */
#define LWB_SCHED_IS_2ND(s)           (((s)->period & 0x8000) == 0)
/**
 * @brief returns the number of data slots from schedule
 */
#define LWB_SCHED_N_SLOTS(s)          ((s)->n_slots & 0x1fff)
/**
 * @brief checks whether schedule has data slots
 */
#define LWB_SCHED_HAS_DATA_SLOT(s)    (((s)->n_slots & 0x3fff) > 0)
/**
 * @brief checks whether schedule has a contention slot
 */
#define LWB_SCHED_HAS_CONT_SLOT(s)    (((s)->n_slots & 0x4000) > 0)
/**
 * @brief checks whether schedule has an S-ACK slot
 */
#define LWB_SCHED_HAS_SACK_SLOT(s)    (((s)->n_slots & 0x8000) > 0)
/**
 * @brief checks whether schedule has a D-ACK slot
 */
#define LWB_SCHED_HAS_DACK_SLOT(s)    (((s)->n_slots & 0x2000) > 0)
/**
 * @brief marks schedule to have a contention slot
 */
#define LWB_SCHED_SET_CONT_SLOT(s)    ((s)->n_slots |= 0x4000)
/**
 * @brief marks schedule to have an S-ACK slot
 */
#define LWB_SCHED_SET_SACK_SLOT(s)    ((s)->n_slots |= 0x8000)
/**
 * @brief marks schedule to have a D-ACK slot
 */
#define LWB_SCHED_SET_DACK_SLOT(s)    ((s)->n_slots |= 0x2000)

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
    void GetSlots(uint16_t slot[LWB_MAX_DATA_SLOTS]);
    
    void SetSACK(bool has);
    bool HasSACK(void) const;
    
    void SetDACK(bool has);
    bool HasDACK(void) const;
    
    void SetCONT(bool has);
    bool HasCONT(void) const;
    
private:
    uint32_t m_time;
    uint16_t m_period;
    uint16_t m_n_slots;
    uint16_t m_slot[LWB_MAX_DATA_SLOTS];//ids
    
    bool    m_hasSACK;
    bool    m_hasDACK;
    bool    m_hasCONT;
};

}
}

#endif /*   __LWB_SCEDULE_PACKET_HEADER_H__    */
