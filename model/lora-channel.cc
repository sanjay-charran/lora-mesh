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
        .SetGroupName ("lora_mesh");
        
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



}
}
