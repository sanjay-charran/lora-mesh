/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __PCAP_HELPER_FOR_LORA_H__
#define __PCAP_HELPER_FOR_LORA_H__

#include "ns3/trace-helper.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"

#include "ns3/lora-net-device.h"
#include "ns3/lora-phy.h"

#include <fstream>

namespace ns3 {
namespace lora_mesh {

class PcapHelperForLoRa : public PcapHelperForDevice
{
public:

    PcapHelperForLoRa();
    ~PcapHelperForLoRa();
    
    void EnablePcapInternal(std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename);
    
protected:

    static void PcapRxSniffer(Ptr<PcapFileWrapper> file, Ptr<Packet> packet);
    static void PcapTxSniffer(Ptr<PcapFileWrapper> file, Ptr<Packet> packet);

private:
    
};
    
}
}

#endif /*   __PCAP_HELPER_FOR_LORA_H__  */
