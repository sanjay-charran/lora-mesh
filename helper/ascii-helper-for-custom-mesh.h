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

#ifndef __ASCII_HELPER_FOR_CUSTOM_MESH_H__
#define __ASCII_HELPER_FOR_CUSTOM_MESH_H__

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

class AsciiHelperForCustomMesh : public AsciiTraceHelperForDevice
{
public:
    AsciiHelperForCustomMesh();
    ~AsciiHelperForCustomMesh();
    
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

#endif  /*  __ASCII_HELPER_FOR_CUSTOM_MESH_H__ */
