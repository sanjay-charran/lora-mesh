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
 * 
 * Modified by: Sanjay Charran <sanjaycharran@gmail.com>
 */

#ifndef __LORA_PHY_H__
#define __LORA_PHY_H__

#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/nstime.h"

#include "ns3/lora-channel.h"
#include "ns3/lora-net-device.h"
#include "ns3/lora-mac.h"
#include "ns3/lora-interference-helper.h"

#define MINIMUM_LORA_SPREADING_FACTOR   6
#define MAXIMUM_LORA_SPREADING_FACTOR   12

namespace ns3 {
namespace lora_mesh {
    
class LoRaMAC;
class LoRaChannel;    

/**
 *  Enumerated type with the possible states of the LoRaPHY
 */
enum PHYState
{
    SLEEP,
    TX,
    RX,
    STANDBY
};

/**
 *  \brief Class representing the Physical layer of a LoRa devive
 * 
 *  Class contains the Physical layer properties of a LoRa device, controlling the various tx/rx
 *  parameters used and the current state of the device. Modified from https://github.com/signetlabdei/lorawan 
 */
class LoRaPHY : public Object
{
public:

    LoRaPHY();
    ~LoRaPHY();
    
    static TypeId GetTypeId(void);
    
    /**
     *  Attaches a LoRaNetDevice to this LoRaPHY
     * 
     *  \param  lnd pointer to the LoRaNetDevice to be attached
     */
    void SetNetDevice(Ptr<LoRaNetDevice> lnd);
    
    /**
     *  Gets the attached LoRaNetDevice to this LoRaPHY
     * 
     *  \return pointer to the attached LoRaNetDevice
     */
    Ptr<LoRaNetDevice> GetNetDevice(void) const;
    
    /**
     *  Attaches a LoRaChannel to this LoRaPHY
     * 
     *  \param  c   pointer to the LoRaChannel to be attached
     */
    void SetChannel(Ptr<LoRaChannel> c);
    
    /**
     *  Gets the LoRaChannel attached to this LoRaPHY
     * 
     *  \return pointer to ths attached LoRaChannel
     */
    Ptr<LoRaChannel> GetChannel(void) const;
    
    /**
     *  Attaches a LoRaMAC to this LoRaPHY
     * 
     *  \param  mac pointer to the LoRaMAC to be attached
     */
    void SetMAC(Ptr<LoRaMAC> mac);
    
    /**
     *  Gets the attached LoRaMAC to this LoRaPHY
     * 
     *  \return pointer to the attached LoRaMAC
     */
    Ptr<LoRaMAC> GetMAC(void) const;
    
    /**
     *  Sets the mobility model used by this LoRaPHY
     * 
     *  \param  mobility    pointer to the MobilityModel to be used by the LoRaPHY
     */
    void SetMobility(Ptr<MobilityModel> mobility);
    
    /**
     *  Gets the currently used mobility model by this LoRaPHY
     * 
     *  \return pointer to the curerntly used MobilityMode
     */
    Ptr<MobilityModel> GetMobility(void) const;
    
    /**
     *  Sets the transmission power used by this LoRaPHY
     * 
     *  \param  power_dBm   the transmission power (dBm) to be set
     */
    void SetTxPower(double power_dBm);
    
    /**
     *  Gets the transmission power currently being used by the LoRaPHY
     * 
     *  \return the transmission power (dBm) being used by the LoRaPHY
     */
    double GetTxPower(void) const;
    
    /**
     *  Sets the frequency being used for transmission by this LoRaPHY
     * 
     *  \param  freq_MHz    the frequency (MHz) to be set
     */
    void SetTxFreq(double freq_MHz);

    /**
     *  Gets the frequency currently being used for transmission by this LoRaPHY
     * 
     *  \return the frequency (MHz) currently being used for transmission
     */
    double GetTxFreq(void) const;
    
    /**
     *  Sets the spreading factor being used for transmission
     * 
     *  \param  sf  the spreading factor to be set
     */
    void SetTxSF(uint8_t sf);

    /**
     *  Gets the spreading factor currently being used for transmission
     * 
     *  \return the spreading factor currently being used for transmission
     */
    uint8_t GetTxSF(void) const;
    
    /**
     *  Sets the bandwidth to be used for transmission
     * 
     *  \param  bw_Hz   the bandwidth (Hz) for transmission to be set
     */
    void SetTxBW(double bw_Hz);

    /**
     *  Gets the bandwidth curerntly being used for transmission
     * 
     *  \return the bandwidth (Hz) currently being used for transmission
     */
    double GetTxBW(void) const;
    
    /**
     *  Sets the number of preambles to be sent before a LoRa signal
     * 
     *  \param  num_preamples the number of preambles to be set
     */
    void SetTxPreamples(uint32_t num_preamples);

    /**
     *  Gets the number of preambles currently being used for transmission
     *
     *  \return the number of preambles being used for transmission
     */
    uint32_t GetTxPreamples(void) const;
    
    /**
     *  Sets the coding rate to be used for transmission
     * 
     *  \param  coding_rate the coding_rate to be set
     */
    void SetTxCodingRate(uint8_t coding_rate);
    
    /**
     *  Gets the coding rate currently being used for transmission
     * 
     *  \return the coding rate currently being used for transmission
     */
    uint8_t GetCodingRate(void) const;
    
    /**
     *  Toggles whether the header is enabled or disabled
     */
    void ToggleHeader(void);
    
    /**
     *  Checks whether the PHY layer header is disabled or not
     * 
     *  \return true if the header is disabled, false otherwise
     */
    bool IsHeaderDisabled(void) const;
    
    /**
     *  Toggles whether the CRC is enabled or disabled
     */
    void ToggleCRC(void);
    
    /**
     *  Checks whether CRC is enabled or not
     * 
     *  \return true if CRC is enabled, false otherwise
     */
    bool IsCRCEnabled(void) const;
    
    /**
     *  Toggles whether low data rate optimization is enabled or disabled
     */
    void ToggleLowDataRateOpt(void);
    
    /**
     *  Checks whether low data rate optimization is enaled or not
     * 
     *  \return true if low data rate optimization is enabled, false otherwise
     */
    bool IsLowDataRateOptEnabled(void) const;
    
    /**
     *  Sets the receiver sensitivity
     *
     *  \param  sens_dBm    the receiver sensitivity (dBm) to be set 
     */
    void SetRxSens(double sens_dBm);
    
    /**
     *  Gets the receiver sensitivity currently being used
     * 
     *  \return the receiver sensitivity currently being used
     */
    double GetRxSens(void) const;
    
    /**
     *  Sets the frequency the receiver listens for
     * 
     *  \param  freq_MHz    the frequency (MHz) to be set
     */
    void SetRxFreq(double freq_MHz);
    
    /**
     *  Gets the frequency currently being listened for by the receiver
     * 
     *  \return the frequency (MHz) currently being listened for by the receiver
     */
    double GetRxFreq(void) const;
    
    /**
     *  Sets the spreading factor being liistened for
     * 
     *  \param  sf  the spreading factor to be set
     */
    void SetRxSF(uint8_t sf);

    /**
     *  Gets the spreading factor currently being listened for
     * 
     *  \return the spreading factor currently being listened for
     */
    uint8_t GetRxSF(void) const;
    
    /**
     *  Switches the state of the LoRaPHY to SLEEP
     */
    void SwitchStateSLEEP(void);
    
    /**
     *  Switches the state of the LoRaPHY to STANDBY
     */
    void SwitchStateSTANDBY(void);
    
    /**
     *  Gets the current state of the LoRaPHY
     * 
     *  \return the current state of the LoRaPHY
     */
    PHYState GetState(void) const;
    
    /**
     *  Sends a packet through an attached channel
     *
     *  \param  packet  pointer to the packet to be sent 
     */
    void Send(Ptr<Packet> packet);
    
    /**
     *  Begins the receiving process for a packet at the PHY layer, inclusive of checking for 
     *  packet collisions
     * 
     *  \param  packet          pointer to the packet being received
     *  \param  duration        time for the packet to be received from start to end
     *  \param  sf              the spreading factor used for the transmission of the packet
     *  \param  rx_power_dBm    the power (dBm) of the packet's signal at the receiver
     *  \param  freq_MHz        the frequency (MHz) the packet was transmitted on
     */
    void StartReceive(Ptr<Packet> packet, Time duration, uint8_t sf, double rx_power_dBm, double freq_MHz);
    
    /**
     *  Ends the process of receiving a packet
     * 
     *  \param  packet  pointer to the packet being received
     */
    void EndReceive(Ptr<Packet> packet, Ptr<LoraInterferenceHelper::Event> event);
    
    /**
     *  Computes the on-air time for a packet based on the tx paramters of this LoRaPHY
     * 
     *  \param  packet  pointer to the packet to calculate the on-air time for
     *  
     *  \return the calculated on-air time of the packet being sent by this LoRaPHy
     */
    Time GetOnAirTime(Ptr<Packet> packet);
    
private:
    /**
     *  Switches the state of the LoRaPHY to TX
     */
    void SwitchStateTX(void);
    
    /**
     *  Switches the state of the LoRaPHY to RX
     */
    void SwitchStateRX(void);
    
    PHYState            m_state;
    Ptr<LoRaNetDevice>  m_device;
    Ptr<LoRaChannel>    m_channel;
    Ptr<LoRaMAC>        m_mac;
    
    Ptr<MobilityModel> m_mobility;
    LoraInterferenceHelper m_interference;
    
    /*  transmit parameters */
    double      m_tx_power_dBm;
    double      m_tx_freq_MHz;
    uint8_t     m_tx_sf;
    double      m_tx_bandwidth_Hz;
    uint32_t    m_tx_numPreambles;
    uint8_t     m_tx_codingRate;
    bool        m_tx_headerDisabled;
    bool        m_crcEnabled;
    bool        m_lowDataRateOpt;
    
    /*  receiver parameters   */
    double  m_rx_sens_dBm;
    double  m_rx_freq_MHz;
    uint8_t m_rx_sf;
    
    TracedCallback<Ptr<const Packet>, uint32_t> m_startSending;
    TracedCallback<Ptr<const Packet>>           m_phyRxBeginTrace;
    TracedCallback<Ptr<const Packet>>           m_phyRxEndTrace;
    TracedCallback<Ptr<const Packet>, uint32_t> m_successfullyReceivedPacket;
    TracedCallback<Ptr<const Packet>, uint32_t> m_underSensitivity;
    TracedCallback<Ptr<const Packet>, uint32_t> m_interferedPacket;
    TracedCallback<Ptr<const Packet>>           m_rxSniffer;
    TracedCallback<Ptr<const Packet>>           m_txSniffer;
};
    
}
}
#endif /*   __LORA_PHY_H__    */
