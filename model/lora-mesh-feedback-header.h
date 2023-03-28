/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __LORA_MESH_FEEDBACK_HEADER__
#define __LORA_MESH_FEEDBACK_HEADER__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {

/**
 *  \brief  Packet header to be attached to LoRa mesh feedback packets
 *  
 *  This header is used to add the packet ID of the packet for which the feedback packet was
 *  made which is not included in the general LoRa mesh header.
 */
class LoRaMeshFeedbackHeader : public Header
{
public:
    
    LoRaMeshFeedbackHeader ();
    ~LoRaMeshFeedbackHeader ();
    
    static TypeId GetTypeId (void);
    TypeId GetInstanceTypeId (void) const;
    
    /*  virtual funcs   */
    uint32_t GetSerializedSize (void) const;
    uint32_t Deserialize (Buffer::Iterator start);
    void Serialize (Buffer::Iterator start) const;
    void Print (std::ostream &os) const;
    
    /**
     * Sets the packet ID
     * 
     * \param   packet_id   the packet ID to be set
     */
    void SetPacketId (uint64_t packet_id);
    
    /**
     *  Gets the packet ID being used by the header
     * 
     *  \return the packet ID being used by the header
     */
    uint64_t GetPacketId (void) const;
    
private:
    uint64_t m_packetid;
};
    
}
}

#endif /*   __LORA_MESH_FEEDBACK_HEADER__   */
