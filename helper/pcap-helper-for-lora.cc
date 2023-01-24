/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/pcap-helper-for-lora.h"

namespace ns3 {
namespace lora_mesh {

PcapHelperForLoRa::PcapHelperForLoRa()
{
}

PcapHelperForLoRa::~PcapHelperForLoRa()
{
}

void
PcapHelperForLoRa::EnablePcapInternal(std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename)
{
    Ptr<LoRaNetDevice> device = nd->GetObject<LoRaNetDevice>();
    
    if (device == 0)
    {
        //wrong type of device
        return;
    }

    Ptr<LoRaPHY> phy = device->GetPHY();
    
    if (phy == 0)
    {
        //no phy
        return;
    }
    
    PcapHelper helper;
    std::string filename;
    
    if (explicitFilename)
    {
        filename = prefix;
    }
    else
    {
        filename = helper.GetFilenameFromDevice (prefix, device);
    }
    
    Ptr<PcapFileWrapper> file = helper.CreateFile(filename, std::ios::out, PcapHelper::DLT_NULL);   //this is an error which may need to be changed
    phy->TraceConnectWithoutContext ("RxSniffer", MakeBoundCallback (&PcapHelperForLoRa::PcapRxSniffer, file));
    phy->TraceConnectWithoutContext ("TxSniffer", MakeBoundCallback (&PcapHelperForLoRa::PcapTxSniffer, file));
    return;
}
    
void
PcapHelperForLoRa::PcapRxSniffer(Ptr<PcapFileWrapper> file, Ptr<Packet> packet)
{
    file->Write(Simulator::Now(), packet);
    return;
}
  
void
PcapHelperForLoRa::PcapTxSniffer(Ptr<PcapFileWrapper> file, Ptr<Packet> packet)
{
    file->Write(Simulator::Now(), packet);
    return;
}
    
}
}
