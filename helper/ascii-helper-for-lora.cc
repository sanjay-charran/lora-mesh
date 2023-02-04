/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/ascii-helper-for-lora.h"

namespace ns3 {
namespace lora_mesh {
    
AsciiHelperForLoRa::AsciiHelperForLoRa()
{
    m_sent = 0;
    m_received = 0;
    
    Simulator::ScheduleDestroy(&AsciiHelperForLoRa::DisplayPDR, this);
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
    
    mac->TraceConnectWithoutContext("RxPacketSniffer", MakeBoundCallback(&AsciiHelperForLoRa::AsciiRxSniffer, this, osw, device));
    
    mac->TraceConnectWithoutContext("TxPacketSniffer", MakeBoundCallback(&AsciiHelperForLoRa::AsciiTxSniffer, this, osw, device));
    
    return;
}
 
void
AsciiHelperForLoRa::AsciiRxSniffer(AsciiHelperForLoRa *ascii, Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet)
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
    
    if (header.GetDest() == device->GetNode()->GetId())
    {
        /*  reached destination */
        ascii->IncrementReceived();
    }
    
    return;
}
 
void
AsciiHelperForLoRa::AsciiTxSniffer(AsciiHelperForLoRa *ascii, Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet)
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
    
    if (header.GetSrc() == header.GetFwd())
    {
        /*  packet starting point   */
        ascii->IncrementSent();
    }
    
    return;
}
 
void
AsciiHelperForLoRa::IncrementSent(void)
{
    m_sent++;
    return;
}

void
AsciiHelperForLoRa::IncrementReceived(void)
{
    m_received++;
    return;
}

 
void
AsciiHelperForLoRa::DisplayPDR(void)
{
    AsciiTraceHelper ascii;
    std::string filename = "pdr.tr";
    Ptr<OutputStreamWrapper> osw = ascii.CreateFileStream (filename);
    std::ostream *os = osw->GetStream();
    
    double pdr = ((double)m_received)/m_sent;
    
    *os << "Total Packets Sent = " << m_sent << std::endl;
    *os << "Total Packets Received = " << m_received << std::endl;
    *os << "Calculated PDR = " << pdr << std::endl;
    
    return;
}
 
}
}
