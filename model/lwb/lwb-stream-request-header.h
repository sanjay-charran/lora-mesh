#ifndef __LWB_STREAM_REQUEST_HEADER_H__
#define __LWB_STREAM_REQUEST_HEADER_H__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {

class LWBStreamRequestHeader : public Header
{
public:
    LWBStreamRequestHeader();
    ~LWBStreamRequestHeader();
    
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
    
    void SetIPI(uint16_t ipi);
    uint16_t GetIPI(void) const;
    
private:
    uint16_t    m_node_id;
    //uint8_t     m_reserved;
    uint8_t     m_stream_id;
    uint16_t    m_ipi;//
};

}
}

#endif  /*  __LWB_STREAM_REQUEST_HEADER_H__ */
