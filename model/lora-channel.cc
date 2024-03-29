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

#include "ns3/simulator.h"
#include "ns3/log.h"

#include "ns3/lora-channel.h"

namespace ns3 {
namespace lora_mesh {

NS_LOG_COMPONENT_DEFINE ("LoRaChannel");
    
TypeId
LoRaChannel::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::LoRaChannel")
        .SetParent<Channel> ()
        .SetGroupName ("lora_mesh")
        .AddTraceSource("PacketSent",
                        "Trace source fired whenever a packet goes out on the channel",
                        MakeTraceSourceAccessor (&LoRaChannel::m_packetSent),
                        "ns3::Packet::TracedCallback");
        
    return tid;
}

LoRaChannel::LoRaChannel ()
{
}

LoRaChannel::~LoRaChannel ()
{
}

Ptr<NetDevice> 
LoRaChannel::GetDevice (std::size_t i) const
{
    std::deque<Ptr<LoRaPHY>>::const_iterator iter;
    
    /*  considering first device as 0th */
    if (m_phyList.size() >= i)
    {
        /*  outside of range of list    */
        Ptr<LoRaNetDevice> err = Ptr<LoRaNetDevice>();
        
        return err;
    }
    else
    {
        //std::deque<Ptr<LoRaPHY>>::const_iterator c_iter = m_phyList.begin();
        
        
        iter = m_phyList.begin();// + std::distance<std::deque<Ptr<LoRaPHY>>::const_iterator>(iter, c_iter);
        std::advance(iter, i);
        return (*iter)->GetNetDevice();
    }
}

std::size_t 
LoRaChannel::GetNDevices (void) const
{
    return m_phyList.size();
}

void
LoRaChannel::AddPHY (Ptr<LoRaPHY> phy)
{
    NS_LOG_FUNCTION (this << phy);
    
    std::deque<Ptr<LoRaPHY>>::iterator iter = m_phyList.begin();
    
    uint32_t id;
    
    if (phy->GetNetDevice() && phy->GetNetDevice()->GetNode())
    {
        id = phy->GetNetDevice()->GetNode()->GetId();
            
        for (;iter != m_phyList.end();++iter)
        {
            if ((*iter)->GetNetDevice() && (*iter)->GetNetDevice()->GetNode())
            {
                if ((*iter)->GetNetDevice()->GetNode()->GetId() > id)
                {
                    /*  add LoRaPHYs sorted by node ids if possible */
                    m_phyList.insert (iter, phy);
                    return;
                }
            }
        } 
        
        if (iter == m_phyList.end())
        {
            m_phyList.insert (iter, phy);
            return;
        }
    }
    else 
    {
        m_phyList.insert (m_phyList.end(), phy);
    }
    
    return;
}

void
LoRaChannel::RemovePHY (Ptr<LoRaPHY> phy)
{
     NS_LOG_FUNCTION (this << phy);
    
    std::deque<Ptr<LoRaPHY>>::iterator iter = m_phyList.begin();
    
    for (;iter != m_phyList.end();iter++)
    {
        if ((*iter) == phy)
        {
            /*  if LoRaPHY is found remove it   */
            m_phyList.erase(iter);
            return;
        }
    }
    
    
    return;
}

void
LoRaChannel::SetLossModel (Ptr<PropagationLossModel> loss)
{
    m_lossModel = loss;
    return;
}

Ptr<PropagationLossModel> 
LoRaChannel::GetLossModel (void) const
{
    return m_lossModel;
}

void
LoRaChannel::SetDelayModel (Ptr<PropagationDelayModel> delay)
{
    m_delayModel = delay;
    return;
}

Ptr<PropagationDelayModel>
LoRaChannel::GetDelayModel (void) const
{
    return m_delayModel;
}

void
LoRaChannel::Send (Ptr<LoRaPHY> sender, Ptr<Packet> packet, double tx_power_dBm, double tx_freq_MHz, uint8_t tx_sf, Time dur)
{
    NS_LOG_FUNCTION(this << packet << sender);
    
    Ptr<MobilityModel> sender_mobility = sender->GetMobility();
    Ptr<MobilityModel> receiver_mobility;
    Time delay;
    double rx_power_dBm;
    uint32_t dst_id;
    
    std::deque<Ptr<LoRaPHY>>::iterator i = m_phyList.begin();
    
    for (;i != m_phyList.end();++i)
    {
        if (sender != (*i))
        {
            receiver_mobility = (*i)->GetMobility();
            
            /*  get delay for packet to arrive at receiver  */
            delay = m_delayModel->GetDelay(sender_mobility, receiver_mobility);
            
            rx_power_dBm = GetRxPower (tx_power_dBm, sender_mobility, receiver_mobility);
            
            if ((*i)->GetNetDevice())
            {
                dst_id = (*i)->GetNetDevice()->GetNode()->GetId();
            }
            else
            {
                dst_id = 0;
            }
            
            Simulator::ScheduleWithContext (dst_id, delay, &LoRaChannel::Receive, this, (*i), packet, rx_power_dBm, tx_freq_MHz, tx_sf, dur);
        
            m_packetSent(packet);
        }
    }
    
    return;
}

void
LoRaChannel::Receive (Ptr<LoRaPHY> receiver, Ptr<Packet> packet, double rx_power_dBm, double rx_freq_MHz, uint8_t rx_sf, Time dur)
{
    NS_LOG_FUNCTION(this << packet << receiver);
    
    receiver->StartReceive(packet, dur, rx_sf, rx_power_dBm, rx_freq_MHz);
    
    return;
}

double 
LoRaChannel::GetRxPower (double tx_power_dBm, Ptr<MobilityModel> sender_mobility, Ptr<MobilityModel> receiver_mobility)
{
    return m_lossModel->CalcRxPower(tx_power_dBm, sender_mobility, receiver_mobility);
}

}
}
