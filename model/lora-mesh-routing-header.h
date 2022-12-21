/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __LORA_MESH_ROUTING_HEADER_H__
#define __LORA_MESH_ROUTING_HEADER_H__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {

class LoRaMeshRoutingHeader : public Header
{
public:

    LoRaMeshRoutingHeader ();
    ~LoRaMeshRoutingHeader ();
    
    static TypeId GetTypeId (void);
    TypeId GetInstanceTypeId (void) const;
    
    void SetETX (float etx);
    float GetETX (void) const;
    
    void SetLast (uint8_t last);
    uint8_t GetLast (void) const;
    
    uint32_t GetSerializedSize (void) const;
    
    uint32_t Deserialize (Buffer::Iterator start);
    void Serialize (Buffer::Iterator start) const;
    
    void Print (std::ostream &os) const;
    
private:
    float m_etx;
    uint8_t m_last;
};

}
}

#endif  /*  __LORA_MESH_ROUTING_HEADER_H__  */
