#ifndef __LORA_CHANNEL_H__
#define __LORA_CHANNEL_H__

#include <list>
#include <iterator>

#include "ns3/log.h"
#include "ns3/channel.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/mobility-model.h"
#include "ns3/propagation-delay-model.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-net-device.h"

#include <deque>
#include <iterator>

namespace ns3 {
namespace lora_mesh {

class LoRaPHY;
    
class LoRaChannel : public Channel
{
public:
    LoRaChannel ();
    ~LoRaChannel ();
    
    static TypeId GetTypeId (void);
    
    Ptr<NetDevice> GetDevice (std::size_t i) const;
    std::size_t GetNDevices (void) const;
    void AddPHY (Ptr<LoRaPHY> phy);
    void RemovePHY (Ptr<LoRaPHY> phy);
    
    void SetLossModel (Ptr<PropagationLossModel> loss);
    Ptr<PropagationLossModel> GetLossModel (void) const;
    
    void SetDelayModel (Ptr<PropagationDelayModel> delay);
    Ptr<PropagationDelayModel> GetDelayModel (void) const;
    
    void Send (Ptr<LoRaPHY> sender, Ptr<Packet> packet, double tx_power_dBm, double tx_freq_MHz, uint8_t tx_sf, Time dur);
    double GetRxPower (double tx_power_dBm, Ptr<MobilityModel> sender_mobility, Ptr<MobilityModel> receiver_mobility); 
    
private:
    Ptr<PropagationLossModel> m_lossModel;
    Ptr<PropagationDelayModel> m_delayModel;
    
    std::deque<Ptr<LoRaPHY>> m_phyList;
    
    void Receive (Ptr<LoRaPHY> receiver, Ptr<Packet> packet, double rx_power_dBm, double rx_freq_MHz, uint8_t rx_sf, Time dur);
    
    TracedCallback<Ptr<const Packet> > m_packetSent;
};

}
}

#endif /*   __LORA_CHANNEL_H__ */
