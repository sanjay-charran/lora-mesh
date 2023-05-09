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

#include "ns3/simulator.h"

#include "ns3/lora-mac.h"

namespace ns3 {
namespace lora_mesh {
 
NS_LOG_COMPONENT_DEFINE("LoRaMAC");
    
TypeId 
LoRaMAC::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LoRaMAC")
        .SetParent<Object>()
        .SetGroupName("lora_mesh");
        
    return tid;
}

LoRaMAC::LoRaMAC()
{
}

LoRaMAC::~LoRaMAC()
{
}

void
LoRaMAC::SetPHY(Ptr<LoRaPHY> phy)
{
    m_phy = phy;
    
    return;
}

Ptr<LoRaPHY>
LoRaMAC::GetPHY (void) const
{
    return m_phy;
}

void
LoRaMAC::SetDevice (Ptr<LoRaNetDevice> device)
{
    m_device = device;
    
    if (m_phy && m_phy->GetMobility())
    {
        Vector3D pos = m_phy->GetMobility()->GetPosition();
        NS_LOG_INFO("Node #" << GetId() << "(x=" << pos.x << " y=" << pos.y << " z=" << pos.z << ")");
    }
    
    return;
}

Ptr<LoRaNetDevice> 
LoRaMAC::GetDevice(void) const
{
    return m_device;
}

uint32_t 
LoRaMAC::GetId(void) const
{
    if (m_device)
    {
        return m_device->GetNode()->GetId();
    }
    
    return 0;
}

void 
LoRaMAC::Receive (Ptr<Packet> packet)
{
    NS_LOG_INFO("(receive MAC)Node (x=" << pos.x << " y=" << pos.y << " z=" << pos.z << ") Packet #" << packet->GetUid());
    
    m_device->Receive(packet);
    
    return;
}

void 
LoRaMAC::Send(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);
    
    m_phy->Send(packet);
    
    return;
}

}
}
