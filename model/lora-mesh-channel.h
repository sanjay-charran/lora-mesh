#ifndef __LORA_MESH_CHANNEL_H__
#define __LORA_MESH_CHANNEL_H__

#include <list>

#include "ns3/channel.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/propagation-loss-model.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-net-device.h"

namespace ns3 {
namespace lora_mesh {

class LoRaMeshChannel : public Channel
{
public:
    LoRaMeshChannel ();
    ~LoRaMeshChannel ();
    
    static TypeId GetTypeId (void);
    
    Ptr<LoRaNetDevice> GetDevice (std::size_t i) const;
    std::size_t GetNDevices (void) const;
    
    void SetLossModel (Ptr<PropogationLossModel> loss);
    Ptr<PropogationLossModel> GetLossModel (void) const;
    
    void SetDelayModel (Ptr<PropogationDelayModel> delay);
    Ptr<PropogationDelayModel> GetDelayModel (void) const;
    
    //void Send (Ptr<LoRaNetDevice> sender, Ptr<Packet> packet, );
    //double CalcRxPower (); 
    
private:
    Ptr<PropogationLossModel> m_lossModel;
    Ptr<PropogationDelayModel> m_delayModel;
    
};

}
}

#endif /*   __LORA_MESH_CHANNEL_H__ */
