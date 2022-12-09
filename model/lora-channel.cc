/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/simualator.h"

#include "ns3/lora-channel.h"

namespace ns3 {
namespace lora_mesh {
 
TypeId
LoRaChannel::GetTypeId (void)
{
    static tid = TypeId ("ns3::LoRaChannel")
        .SetParent<Channel> ()
        .SetGroupName ("lora_mesh")
        .AddTraceSource("PacketSent",
                        "Trace source fired whenever a packet goes out on the channel",
                        MakeTraceSourceAccessor (&LoraChannel::m_packetSent),
                        "ns3::Packet::TracedCallback");
        
    return tid;
}

LoRaChannel::LoRaChannel ()
{
}

LoRaChannel::~LoRaChannel ()
{
}

Ptr<LoRaNetDevice> 
LoRaChannel::GetDevice (std::size_t i) const
{
    list<Ptr<LoRaPHY>>::iterator iter;
    
    /*  considering first device as 0th */
    if (m_phyList.size() >= i)
    {
        /*  outside of range of list    */
        return Ptr<LoRaNetDevice>::Ptr(nullptr);
    }
    else
    {
        iter = m_phyList.begin();
        advance(iter, i);
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
    list<Ptr<LoRaPHY>>::iterator iter;
    uint32_t id;
    
    if (phy->GetNetDevice() && phy->GetNetDevice()->GetNode())
    {
        id = phy->GetNetDevice()->GetNode()->GetId();
            
        for (iter = m_phyList.begin();iter != m_phyList.end();iter++)
        {
            if ((*iter)->GetNetDevice() && (*iter)->GetNetDevice()->GetNode())
            {
                if ((*iter)->GetNetDevice()->GetNode()->GetId() > id)
                {
                    m_phyList.insert (iter, phy);
                    return;
                }
            }
            else
            {
                m_phyList.insert (iter, phy);
                return;
            }
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
    m_phyList.remove(phy);
    return;
}

void
LoRaChannel::SetLossModel (Ptr<PropogationLossModel> loss)
{
    m_lossModel = loss;
    return;
}

Ptr<PropogationLossModel> 
LoRaChannel::GetLossModel (void) const
{
    return m_lossModel;
}

void
LoRaChannel::SetDelayModel (Ptr<PropogationDelayModel> delay)
{
    m_delayModel = delay;
    return;
}

Ptr<PropogationDelayModel>
LoRaChannel::GetDelayModel (void) const
{
    return m_delayModel;
}

void
LoRaChannel::Send (Ptr<LoRaPHY> sender, Ptr<Packet> packet, double tx_power_dBm, double tx_freq_MHz, uint8_t tx_sf, Time dur)
{
    Ptr<MobilityModel> sender_mobility = sender->GetMobility();
    Ptr<MobilityModel> receiver_mobility;
    Time delay;
    double rx_power_dBm;
    uint32_t dst_id;
    
    std::list<Ptr<LoRaPHY>>::iterator i;
    
    for (i = m_phyList.begin();i != m_phyList.end();i++)
    {
        if (sender != (*i))
        {
            receiver_mobility = (*i)->GetMobility();
            delay = m_delayModel->GetDelay(sender_mobility, receiver_mobility);
            rx_power_dBm = GetRxPower (tx_power_dBm, sender_mobility, receiver_mobility);
            
            if ((*i)->GetDevice() != 0)
            {
                dst_id = (*i)->GetDevice()->GetNode()->GetId();
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
