#ifndef __LWB_STREAM_ACK_HEADER_H__
#define __LWB_STREAM_ACK_HEADER_H__

#include "ns3/header.h"

#define LWB_SACK_MIN_PKT_LEN    4

namespace ns3 {
namespace lora_mesh {

class LWBStreamACKHeader : public Header
{
public:
    LWBStreamACKHeader();
    ~LWBStreamACKHeader();
    
    static TypeId GetTypeId(void);
    TypeId GetInstanceTypeId(void) const;
    
    /*  virtual funcs   */
    uint32_t GetSerializedSize(void) const;
    uint32_t Deserialize(Buffer::Iterator start);
    void Serialize(Buffer::Iterator start) const;
    void Print(std::ostream &os) const;
    
    void SetNodeId(uint16_t node_id);
    uint16_t GetNodeId(void) const;
    
    void SetStreamId(uint8_t stream_id);
    uint8_t GetStreamId(void) const;
    
private:
    uint16_t    m_node_id;
    uint8_t     m_stream_id;
    //uint8_t     m_n_extra_sacks;
    //uint8_t     m_extra[];
};

}
}

#endif /*   __LWB_STREAM_ACK_HEADER_H__ */
