/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2023 Sanjay Charran
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Sanjay Charran <sanjaycharran@gmail.com>
 */

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
    CustomMeshHeader header;
    std::ostream *os = stream->GetStream();
    Vector3D pos = device->GetNode()->GetObject<MobilityModel>()->GetPosition();
    
    packet->PeekHeader(header);
    
    *os << Simulator::Now().GetDouble() << "\tNode #" << device->GetNode()->GetId() << ": Received Packet #" << packet->GetUid() << std::endl;
    *os << "\t\tx=" << pos.x << " , y=" << pos.y << " , z=" << pos.z << std::endl;
    *os << "\t\tSrc: " << header.GetSrc() << std::endl;
    *os << "\t\tDest: " << header.GetDest() << std::endl;
    *os << "\t\tFwd: " << header.GetFwd() << std::endl;
    *os << "\t\tData: \"";
    packet->Print(*os);
    *os << "\"" << std::endl << std::endl;
    
    if (header.GetDest() == device->GetNode()->GetId() && !(ascii->PreviouslyReceived(packet->GetUid())))
    {
        /*  reached destination */
        ascii->IncrementReceived(packet);
    }
    
    return;
}
 
void
AsciiHelperForLoRa::AsciiTxSniffer(AsciiHelperForLoRa *ascii, Ptr<OutputStreamWrapper> stream, Ptr<LoRaNetDevice> device, Ptr<Packet> packet)
{
    LoRaMeshHeader header;
    std::ostream *os = stream->GetStream();
    Vector3D pos = device->GetNode()->GetObject<MobilityModel>()->GetPosition();
    
    packet->PeekHeader(header);
    
    *os << Simulator::Now().GetDouble() << "\tNode #" << device->GetNode()->GetId() << ": Transmitted Packet #" << packet->GetUid() << std::endl;
    *os << "\t\tx=" << pos.x << " , y=" << pos.y << " , z=" << pos.z << std::endl;
    *os << "\t\tSrc: " << header.GetSrc() << std::endl;
    *os << "\t\tDest: " << header.GetDest() << std::endl;
    *os << "\t\tFwd: " << header.GetFwd() << std::endl;
    *os << "\t\tData: \"";
    packet->Print(*os);
    *os << "\"" << std::endl << std::endl;
    
    if (header.GetSrc() == header.GetFwd() && !(ascii->PreviouslySent(packet->GetUid())))
    {
        /*  packet starting point   */
        ascii->IncrementSent(packet);
    }
    
    return;
}
 
void
AsciiHelperForLoRa::IncrementSent(Ptr<Packet> packet)
{
    m_sent++;
    m_sent_ids.push_back(packet->GetUid());
    return;
}

void
AsciiHelperForLoRa::IncrementReceived(Ptr<Packet> packet)
{
    m_received++;
    m_received_ids.push_back(packet->GetUid());
    return;
}

bool
AsciiHelperForLoRa::PreviouslySent(uint32_t pid)
{
    for (unsigned int i = 0;i < m_sent_ids.size();i++)
    {
        if (m_sent_ids[i] == pid)
        {
            return true;
        }
    }
    
    return false;
}

bool
AsciiHelperForLoRa::PreviouslyReceived(uint32_t pid)
{
    for (unsigned int i = 0;i < m_received_ids.size();i++)
    {
        if (m_received_ids[i] == pid)
        {
            return true;
        }
    }
    
    return false;
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
