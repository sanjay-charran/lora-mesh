/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __LORA_NET_DEVICE_H__
#define __LORA_NET_DEVICE_H__

#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/address.h"
#include "ns3/mac48-address.h"
#include "ns3/net-device.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"

#include "ns3/lora-channel.h"
#include "ns3/lora-phy.h"
#include "ns3/lora-mac.h"

namespace ns3 {
namespace lora_mesh {

class LoRaPHY;
class LoRaMAC;    

/**
 *  \brief  Net Device used to interface with MAC and PHY layers of LoRa mesh module
 * 
 *  This class serves to interface between the Node and MAC and PHY layers of the LoRa mesh moduel
 *  and is expected to be the point where the user is to send and receive packets
 */
class LoRaNetDevice : public NetDevice
{
public:
    LoRaNetDevice ();
    ~LoRaNetDevice ();
    
    static TypeId GetTypeId (void);
    
    /**
     *  Attaches LoRaMAC to this LoRaNetDevice
     * 
     *  \param  mac pointer to the LoRaMAC to be attached
     */
    void SetMAC (Ptr<LoRaMAC> mac);
    
    /**
     *  Gets the attached LoRaMAC
     * 
     *  \return pointer to the attached LoRaMAC
     */
    Ptr<LoRaMAC> GetMAC (void) const;
    
    /**
     *  Attaches LoRaPHY to this LoRaNetDevice
     * 
     *  \param  phy pointer to the LoRaPHY to be attached
     */
    void SetPHY (Ptr<LoRaPHY> phy);
    
    /**
     *  Gets the attached LoRaPHY
     * 
     *  \return pointer to the attached LoRaPHY
     */
    Ptr<LoRaPHY> GetPHY(void) const;
    
    void SendTo (Ptr<Packet> packet, uint32_t dest);
    void Receive (Ptr<Packet> packet);
    
    /*  virtual funcs from NetDevice    */
    void SetNode (Ptr<Node> node);
    Ptr<Node> GetNode (void) const;
    void SetAddress (Address address);
    Address GetAddress (void) const;
    void SetIfIndex (const uint32_t index);
    uint32_t GetIfIndex (void) const;
    bool SetMtu (const uint16_t mtu);
    uint16_t GetMtu (void) const;
    bool IsBridge (void) const;
    bool IsBroadcast (void) const;
    bool IsLinkUp (void) const;
    bool IsMulticast (void) const;
    bool IsPointToPoint (void) const;
    Ptr<Channel> GetChannel (void) const;
    Address GetMulticast (Ipv4Address multicastGroup) const;
    Address GetBroadcast (void) const;
    Address GetMulticast (Ipv6Address addr) const;
    bool NeedsArp (void) const;
    bool Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber);
    bool SendFrom (Ptr<Packet> packet, const Address &source, const Address &dest, uint16_t protocolNumber);
    void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
    void SetReceiveCallback (ReceiveCallback cb);
    bool SupportsSendFrom (void) const;
    void AddLinkChangeCallback (Callback<void> callback);
    
private:
    
    Ptr<Node> m_node;
    Ptr<LoRaPHY> m_phy;
    Ptr<LoRaMAC> m_mac;
    
    //NetDevice::ReceiveCallback m_receiveCallback;
};

}
}

#endif /*   __LORA_NET_DEVICE_H__   */
