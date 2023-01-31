/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/ascii-helper-for-lora.h"

namespace ns3 {
namespace lora_mesh {
    
AsciiHelperForLoRa::AsciiHelperForLoRa()
{
}
 
AsciiHelperForLoRa::~AsciiHelperForLoRa()
{
}

void
AsciiHelperForLoRa::EnableAsciiInternal(Ptr<OutputStreamWrapper> stream, std::string prefix, Ptr<NetDevice> nd, bool explicitFilename)
{
    AsciiTraceHelper ascii;
    std::string filename;
    Ptr<OutputStreamWrapper> osw;
    Ptr<LoRaNetDevice> device = nd->GetObject<LoRaNetDevice>();
    
    if (device == 0)
    {
        return;
    }
    
    Ptr<LoRaMAC> mac = device->GetMAC();
    
    if (mac == 0)
    {
        return;
    }
    
    if (explicitFilename)
    {
        filename = prefix;
    }
    else
    {
        filename = ascii.GetFilenameFromDevice (prefix, device);
    }
    
    if (stream == 0)
    {
        osw = ascii.CreateFileStream (filename);
    }
    else
    {
        osw = stream;
    }
    
    mac->TraceConnectWithoutContext("RxPacketSniffer", MakeBoundCallback(&AsciiHelperForLoRa::AsciiRxSniffer, osw, device));
    
    mac->TraceConnectWithoutContext("TxPacketSniffer", MakeBoundCallback(&AsciiHelperForLoRa::AsciiTxSniffer, osw, device));
    
    return;
}
 
void
AsciiHelperForLoRa::AsciiRxSniffer(Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet)
{
    LoRaMeshHeader header;
    std::ostream *os = stream->GetStream();
    
    packet->PeekHeader(header);
    
    *os << Simulator::Now().GetDouble() << "\tNode #" << device->GetNode()->GetId() << ": Received Packet #" << packet->GetUid() << std::endl;
    *os << "\t\tSrc: " << header.GetSrc() << std::endl;
    *os << "\t\tDest: " << header.GetDest() << std::endl;
    *os << "\t\tFwd: " << header.GetFwd() << std::endl;
    *os << "\t\tData: \"";
    packet->Print(*os);
    *os << "\"" << std::endl << std::endl;
    
    
    return;
}
 
void
AsciiHelperForLoRa::AsciiTxSniffer(Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet)
{
    LoRaMeshHeader header;
    std::ostream *os = stream->GetStream();
    
    packet->PeekHeader(header);
    
    *os << Simulator::Now().GetDouble() << "\tNode #" << device->GetNode()->GetId() << ": Transmitted Packet #" << packet->GetUid() << std::endl;
    *os << "\t\tSrc: " << header.GetSrc() << std::endl;
    *os << "\t\tDest: " << header.GetDest() << std::endl;
    *os << "\t\tFwd: " << header.GetFwd() << std::endl;
    *os << "\t\tData: \"";
    packet->Print(*os);
    *os << "\"" << std::endl << std::endl;
    
    return;
}
 
}
}
