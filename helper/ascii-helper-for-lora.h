/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __ASCII_HELPER_FOR_LORA_H__
#define __ASCII_HELPER_FOR_LORA_H__

#include "ns3/trace-helper.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/simulator.h"

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
    
    void IncrementSent(void);
    void IncrementReceived(void);
private:
    static void AsciiRxSniffer(AsciiHelperForLoRa *ascii, Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet);
    static void AsciiTxSniffer(AsciiHelperForLoRa *ascii, Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet);
    
    void DisplayPDR(void);
    //
    
    uint64_t m_sent;
    uint64_t m_received;
};
    
}
}

#endif  /*  __ASCII_HELPER_FOR_LORA_H__ */
