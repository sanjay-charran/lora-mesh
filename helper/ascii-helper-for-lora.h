/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __ASCII_HELPER_FOR_LORA_H__
#define __ASCII_HELPER_FOR_LORA_H__

#include "ns3/trace-helper.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model.h"
#include "ns3/vector.h"

#include "ns3/lora-net-device.h"
#include "ns3/lora-mac.h"

#include <vector>

namespace ns3 {
namespace lora_mesh {

class AsciiHelperForLoRa : public AsciiTraceHelperForDevice
{
public:
    AsciiHelperForLoRa();
    ~AsciiHelperForLoRa();
    
    void EnableAsciiInternal(Ptr<OutputStreamWrapper> stream, std::string prefix, Ptr<NetDevice> nd, bool explicitFilename);
    
    void IncrementSent(Ptr<Packet> packet);
    void IncrementReceived(Ptr<Packet> packet);
    bool PreviouslySent(uint32_t pid);
    bool PreviouslyReceived(uint32_t pid);
private:
    static void AsciiRxSniffer(AsciiHelperForLoRa *ascii, Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet);
    static void AsciiTxSniffer(AsciiHelperForLoRa *ascii, Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet);
    
    void DisplayPDR(void);
    //
    
    uint64_t m_sent;
    uint64_t m_received;
    std::vector<uint32_t> m_sent_ids;
    std::vector<uint32_t> m_received_ids;
};
    
}
}

#endif  /*  __ASCII_HELPER_FOR_LORA_H__ */
