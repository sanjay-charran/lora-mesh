#ifndef __LORA_MESH_HEADER_H__
#define __LORA_MESH_HEADER_H__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {
 
/**
 *  Enumerated type for the possible types of packets supported
 */
enum MsgType
{
    ROUTING_UPDATE = 0,     /*  broadcasting to update routing tables   */ 
    
    DIRECTED = 1,           /*  packet being sent to only a specific node   */

    FEEDBACK = 2            /*  feedback for packet reception   */
};
    
/**
 *  \brief  Main packet header used for all LoRa mesh packets
 * 
 *  This packet header is used to track information on the packets route, in terms of the 
 *  original source, destination and last forwarding node IDs. Another important parameter in 
 *  the header is the packet type which can be used identify the existence of other LoRa mesh 
 *  headers
 */
class LoRaMeshHeader : public Header
{
public:
    LoRaMeshHeader ();
    ~LoRaMeshHeader ();
    
    static TypeId GetTypeId (void);
    TypeId GetInstanceTypeId (void) const;
    uint32_t GetSerializedSize (void) const;
    uint32_t Deserialize (Buffer::Iterator start);
    void Serialize (Buffer::Iterator start) const;
    void Print (std::ostream &os) const;
    
    /**
     *  Sets the source ID
     * 
     *  \param  src the source ID to be set
     */
    void SetSrc (uint32_t src);
    
    /**
     *  Gets the source ID being used by the header
     * 
     *  \return the source ID being used by the header
     */
    uint32_t GetSrc (void) const;
    
    /**
     *  Sets the destination ID
     * 
     *  \param  dest    the destination ID to be set
     */
    void SetDest (uint32_t dest);
    
    /**
     *  Gets the destination ID being used by the header
     * 
     *  \return the destination ID being used by the header
     */
    uint32_t GetDest (void) const;
    
    /**
     *  Sets the type of packet
     * 
     *  \param  type    the type of packet to be set
     */
    void SetType (MsgType type);
    
    /**
     *  Gets the type of packet being used by the header
     * 
     *  \return the type of packet being used by the header
     */
    MsgType GetType (void) const;
    
    /**
     *  Sets the forwarding ID 
     * 
     *  \param  fwd the forwarding ID to be set
     */
    void SetFwd (uint32_t fwd);

    /**
     *  Gets the forwarding ID being used by the header
     * 
     *  \return the forwarding ID being used by the header
     */
    uint32_t GetFwd (void) const;
    
private:
    MsgType m_type;
    uint32_t m_src;
    uint32_t m_dest;
    uint32_t m_fwd;
};

}
}
#endif /*   __LORA_MESH_HEADER_H__  */
