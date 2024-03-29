/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 University of Padova
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
 * Author: Davide Magrin <magrinda@dei.unipd.it>
 */

#include "ns3/lora-net-device.h"

namespace ns3 {
namespace lora_mesh {
 
NS_LOG_COMPONENT_DEFINE ("LoRaNetDevice");  

TypeId
LoRaNetDevice::GetTypeId (void)
{
    TypeId tid = TypeId ("ns3::LoRaNetDevice")
    .SetParent<NetDevice> ()
    .SetGroupName ("lora_mesh");
    
    return tid;
}
    
LoRaNetDevice::LoRaNetDevice ()
{
}
 
LoRaNetDevice::~LoRaNetDevice ()
{
}

void 
LoRaNetDevice::SetMAC (Ptr<LoRaMAC> mac)
{
    m_mac = mac;
    return;
}

Ptr<LoRaMAC> 
LoRaNetDevice::GetMAC (void) const
{
    return m_mac;
}
 
void 
LoRaNetDevice::SetPHY (Ptr<LoRaPHY> phy)
{
    m_phy = phy;
    return;
}
 
Ptr<LoRaPHY>
LoRaNetDevice::GetPHY(void) const
{
    return m_phy;
}

void
LoRaNetDevice::SendTo (Ptr<Packet> packet, uint32_t dest)
{
    NS_LOG_FUNCTION (this << packet << dest);
    
    if (m_mac != 0)
    {
        m_mac->SendTo(packet, dest);
    }
    
    return;
}

void 
LoRaNetDevice::Receive (Ptr<Packet> packet)
{
     NS_LOG_FUNCTION (this << packet);
    
    //m_receiveCallback (this, packet, 0, Address());
    return;
}

void
LoRaNetDevice::SetNode (Ptr<Node> node)
{
    m_node = node;
    return;
}
 
Ptr<Node>
LoRaNetDevice::GetNode (void) const
{
    return m_node;
}

void
LoRaNetDevice::SetAddress (Address address)
{
    return;
}

Address
LoRaNetDevice::GetAddress (void) const
{
    return Address();
}

bool 
LoRaNetDevice::SetMtu (const uint16_t mtu)
{
    /*  unsupported */
    return false;
}

uint16_t 
LoRaNetDevice::GetMtu (void) const
{
    /*  unsupported */
    return 0;
}

void
LoRaNetDevice::SetIfIndex (const uint32_t index)
{
    return;
}

uint32_t
LoRaNetDevice::GetIfIndex (void) const
{
    return 0;
}

bool 
LoRaNetDevice::IsBridge (void) const
{
    return false;
}

bool 
LoRaNetDevice::IsBroadcast (void) const
{
    return true;
}

bool 
LoRaNetDevice::IsLinkUp (void) const
{
    return (m_phy != 0);
}

bool 
LoRaNetDevice::IsMulticast (void) const
{
    return false;
}

bool
LoRaNetDevice::IsPointToPoint (void) const
{
    return false;
}

Ptr<Channel> 
LoRaNetDevice::GetChannel (void) const
{
    if (m_phy)
    {
        return m_phy->GetChannel();
    }
    else
    {
        return 0;
    }
}

Address
LoRaNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
    /*  unsupported */
    return Address();
}

Address 
LoRaNetDevice::GetBroadcast (void) const
{
    return Address();
}

Address 
LoRaNetDevice::GetMulticast (Ipv6Address addr) const
{
    return Address();
}

bool
LoRaNetDevice::NeedsArp (void) const
{
    return false;
}

bool 
LoRaNetDevice::Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber)
{
    if (m_mac != 0)
    {
        m_mac->Send(packet);
    }
    
    return true;
}

bool
LoRaNetDevice::SendFrom (Ptr<Packet> packet, const Address &source, const Address &dest, uint16_t protocolNumber)
{
    /*  unsupported */
    return false;
}

void 
LoRaNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
    /*  unsupported */
    return;
}

void
LoRaNetDevice::SetReceiveCallback (ReceiveCallback cb)
{
    //m_receiveCallback = cb;
    return;
}

bool 
LoRaNetDevice::SupportsSendFrom (void) const
{
    return false;
}

void
LoRaNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
    /*  unsupported */
    return;
}

}
}
