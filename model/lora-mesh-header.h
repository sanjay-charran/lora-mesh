#ifndef __LORA_MESH_HEADER_H__
#define __LORA_MESH_HEADER_H__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {
 
enum MsgType
{
    ROUTING_UPDATE = 0,     /*  broadcasting changes to routing table   */ 
    
    BROADCAST = 1,          /*  regular broadcasting to all nodes in transmission range */
    
    DIRECTED = 2,           /*  packet being sent to only a specific node   */

    FEEDBACK = 3            /*  feedback for packet reception   */
};
    
class LoRaMeshHeader : public Header
{
public:
    LoRaMeshHeader ();
    ~LoRaMeshHeader ();
    
    static TypeId GetTypeId (void);
    TypeId GetInstanceTypeId (void) const;
    
    void SetSrc (uint32_t src);
    uint32_t GetSrc (void) const;
    
    void SetDest (uint32_t dest);
    uint32_t GetDest (void) const;
    
    void SetType (MsgType type);
    MsgType GetType (void) const;
    
    void SetFwd (uint32_t fwd);
    uint32_t GetFwd (void) const;
    
    uint32_t GetSerializedSize (void) const;
    
    uint32_t Deserialize (Buffer::Iterator start);
    void Serialize (Buffer::Iterator start) const;
    
    void Print (std::ostream &os) const;
    
private:
    MsgType m_type;
    uint32_t m_src;
    uint32_t m_dest;
    uint32_t m_fwd;
};

}
}
#endif /*   __LORA_MESH_HEADER_H__  */
