#ifndef __LWB_DATA_PACKET_HEADER_H__
#define __LWB_DATA_PACKET_HEADER_H__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {

class LWBDataPacketHeader : public header
{
public:
    LWBDataPacketHeader();
    ~LWBDataPacketHeader();
    
    static TypeId GetTypeId(void);
    TypeId GetInstanceTypeId(void) const;
    
    /*  virtual funcs   */
    uint32_t GetSerializedSize(void) const;
    uint32_t Deserialize(Buffer::Iterator start);
    void Serialize(Buffer::Iterator start) const;
    void Print(std::ostream &os) const;
    
    void SetRecipientId(uint16_t rid);
    uint16_t GetRecipientId(void) const;
    
    void SetStreamId(uint8_t sid);
    uint8_t GetStreamid(void) const;
    
private:
    uint16_t    m_recipient_id;
    uint8_t     m_stream_id;
};

}
}

#endif /*   __LWB_DATA_PACKET_HEADER_H__    */
