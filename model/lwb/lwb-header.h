#ifndef __LWB_HEADER_H__
#define __LWB_HEADER_H__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {

typedef enum {
    SCHEDULE = 0,
    DATA = 1,
    STREAM_REQUEST = 2,
    STREAM_ACK = 3,
    DATA_ACK = 4
}lwb_packet_type;
    
class LWBHeader : public Header
{
public:
    LWBHeader();
    ~LWBHeader();
    
    static TypeId GetTypeId(void);
    TypeId GetInstanceTypeId(void) const;
    
    /*  virtual funcs   */
    uint32_t GetSerializedSize(void) const;
    uint32_t Deserialize(Buffer::Iterator start);
    void Serialize(Buffer::Iterator start) const;
    void Print(std::ostream &os) const;
    
    void SetPacketType(lwb_packet_type type);
    lwb_packet_type GetPacketType(void) const;
    
private:
    lwb_packet_type m_type;
};

}
}

#endif  /*  __LWB_STREAM_REQUEST_HEADER_H__ */
