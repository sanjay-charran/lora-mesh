/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __ASCII_HELPER_FOR_LORA_H__
#define __ASCII_HELPER_FOR_LORA_H__

#include "ns3/trace-helper.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"

#include "ns3/lora-net-device.h"
#include "ns3/lora-mac.h"

namespace ns3 {
namespace lora_mesh {

class AsciiHelperForLoRa : public AsciiTraceHelperForDevice
{
public:
    AsciiHelperForLoRa();
    ~AsciiHelperForLoRa();
    
    void EnableAsciiInternal(Ptr<OutputStreamWrapper> stream, std::string prefix, Ptr<NetDevice> nd, bool explicitFilename);
private:
    static void AsciiRxSniffer(Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet);
    static void AsciiTxSniffer(Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet);
};
    
}
}

#endif  /*  __ASCII_HELPER_FOR_LORA_H__ */
