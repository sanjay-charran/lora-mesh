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

#ifndef __LORA_CHANNEL_H__
#define __LORA_CHANNEL_H__

#include <list>
#include <iterator>

#include "ns3/log.h"
#include "ns3/channel.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/mobility-model.h"
#include "ns3/propagation-delay-model.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-net-device.h"

#include <deque>
#include <iterator>

namespace ns3 {
namespace lora_mesh {

class LoRaPHY;

/**
 *  \brief This class allows for attached LoRaPHYs to transmit packets to other attached LoRaPHYs
 * 
 *  This class serves to facilitate the transmission and reception of packets using LoRa between LoRaPHYs. 
 *  It determines which attached LoRaPHYs ought to receive a packet based on the transmission paramters 
 *  (power, spreading factor, etc.) and the propagation loss and delay models chosen to be used.  
 */

class LoRaChannel : public Channel
{
public:
    LoRaChannel ();
    ~LoRaChannel ();
    
    static TypeId GetTypeId (void);
    Ptr<NetDevice> GetDevice (std::size_t i) const;
    std::size_t GetNDevices (void) const;
    
    /**
     *  Attaches LoRaPHYs to the channel enabling the transmission and reception
     *  of packets to and from other LoRaPHYs also attached to this channel.
     *
     *  \param  phy the LoRaPHY to be attached to the channel
     */
    void AddPHY (Ptr<LoRaPHY> phy);
    
    /**
     *  Removes LoRaPHY from the channel, no longer allowing for the transmission and 
     *  reception of packets to and from other LoRaPHYs which are attached to this channel.
     * 
     *  \param  phy the LoRaPHY to be removed from the channel
     */
    void RemovePHY (Ptr<LoRaPHY> phy);
    
    /**
     *  Sets the PropagationLossModel to be used for this channel.
     * 
     *  \param loss pointer to the PropagationLossModel to be used for this channel
     */
    void SetLossModel (Ptr<PropagationLossModel> loss);
    
    /**
     *  Gets the PropagationLossModel that the channel is using.
     *
     *  \return pointer to the PropagationLossModel being used by the channel
     */
    Ptr<PropagationLossModel> GetLossModel (void) const;
    
    /**
     *  Sets the PropagationDelayModel to be used for this channel.
     * 
     *  \param delay pointer to the PropagationDelayModel to be used for this channel
     */
    void SetDelayModel (Ptr<PropagationDelayModel> delay);
    
    /**
     *  Gets the PropagationDelayModel that the channel is using.
     *
     *  \return pointer to the PropagationDelayModel being used by the channel
     */
    Ptr<PropagationDelayModel> GetDelayModel (void) const;
    
    /**
     *  Compares the positions of each LoRaPHY attached to the channel against that of a 
     *  transmitting LoRaPHY to determine the received power and schedules the necessary function
     *  for handling the reception for each LoRaPHY after the time it would take for the packet
     *  to arrive at that LoRaPHY
     *
     *  \param  sender          pointer to the LoRaPHY sending the packet
     *  \param  packet          pointer to the packet being sent
     *  \param  tx_power_dBm    power (in dBm) the packet was transmitted with
     *  \param  tx_freq_MHz     frequency (in MHz) the packet was transmitted on
     *  \param  tx_sf           the spreading factor used for the packet's transmission
     *  \param  dur             the duration it would take to receive the packet itself
     */
    void Send (Ptr<LoRaPHY> sender, Ptr<Packet> packet, double tx_power_dBm, double tx_freq_MHz, uint8_t tx_sf, Time dur);
    
    /**
     *  Computes the power of the packet's signal at the point of the receiver
     * 
     *  \param  tx_power_dBm        power (in dBm) the packet was transmitted with
     *  \param  sender_mobility     pointer to the MobilityModel used by the sending LoRaPHY
     *  \param  receiver_mobility   pointer to the MobilityModel used by the receiving LoRaPHY
     * 
     *  \return the power of the packet's signal at the receiver
     */
    double GetRxPower (double tx_power_dBm, Ptr<MobilityModel> sender_mobility, Ptr<MobilityModel> receiver_mobility); 
    
private:
    Ptr<PropagationLossModel> m_lossModel;
    Ptr<PropagationDelayModel> m_delayModel;
    
    std::deque<Ptr<LoRaPHY>> m_phyList;
    
    /**
     *  Interfaces with the LoRaPHY device receiving the packet passing the necessary parameters
     * 
     *  \param  receiver        pointer to the LoRaPHY receiving the packet
     *  \param  packet          pointer to the packet being received
     *  \param  rx_power_dBm    the power (in dBm) the packet's signal has at the receiver
     *  \param  rx_freq_MHz     the frequency (in MHz) the packet was transmitted on
     *  \param  rx_sf           the spreading factor used for the packet's transmission
     *  \param  dur             the duration it would take to receive the packet itself
     */
    void Receive (Ptr<LoRaPHY> receiver, Ptr<Packet> packet, double rx_power_dBm, double rx_freq_MHz, uint8_t rx_sf, Time dur);
    
    /*
     *  The callback triggered when a packet is sent on the channel
     */
    TracedCallback<Ptr<const Packet> > m_packetSent;
};

}
}

#endif /*   __LORA_CHANNEL_H__ */
