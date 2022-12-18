/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __LORA_MESH_FEEDBACK_HEADER__
#define __LORA_MESH_FEEDBACK_HEADER__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {
 
class LoRaMeshFeedbackHeader : public Header
{
public:
    
    LoRaMeshFeedbackHeader ();
    ~LoRaMeshFeedbackHeader ();
    
    static TypeId GetTypeId (void);
    
    void SetPacketId (uint64_t packet_id);
    uint64_t GetPacketId (void) const;
    
    uint32_t GetSerializedSize (void) const;
    
    uint32_t Deserialize (Buffer::Iterator start);
    void Serialize (Buffer::Iterator start) const;
    
    void Print (std::ostream &os) const;
    
private:
    uint64_t m_packetid;
};
    
}
}

#endif /*   __LORA_MESH_FEEDBACK_HEADER__   */
