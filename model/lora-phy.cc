/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"

#include "ns3/lora-phy.h"

namespace ns3 {
namespace lora_mesh {

TypeId
LoRaPHY::GetTypeId (void)
{
    static tid = TypeId ("ns3::LoRaPHY")
        .SetParent<Object> ()
        .SetGroupName ("lora_mesh");
        
    return tid;
}
    
LoRaPHY::LoRaPHY ()
{
}

LoRaPHY::~LoRaPHY ()
{
}

void 
LoRaPHY::SetNetDevice (Ptr<LoRaNetDevice> lnd)
{
    m_device = lnd;
    return;
}

Ptr<LoRaNetDevice> 
LoRaPHY::GetNetDevice (void) const
{
    return m_device;
}

void 
LoRaPHY::SetChannel (Ptr<LoRaMeshChannel> c)
{
    m_channel = c;
    return;
}

Ptr<LoRaMeshChannel> 
LoRaPHY::GetChannel (void) const
{
    return m_channel;
}

void 
LoRaPHY::SetTxPower (double power_dBm)
{
    m_tx_power_dBm = power_dBm;
    return;
}

double 
LoRaPHY::GetTxPower (void) const
{
    return m_tx_power_dBm;
}

void
LoRaPHY::SetRxSens (double sens_dBm)
{
    m_rx_sens_dBm = sens_dBm;
    return;
}

double
LoRaPHY::GetRxSens (void) const
{
    return m_rx_sens_dBm;
}

void
LoRaPHY::SetTxFreq (double freq_MHz)
{
    m_tx_freq_MHz = freq_MHz;
    return;
}

double
LoRaPHY::GetTxFreq (void) const
{
    return m_tx_freq_MHz;
}

void
LoRaPHY::SetRxFreq (double freq_MHz)
{
    m_rx_freq_MHz = freq_MHz;
    return;
}

double
LoRaPHY::GetRxFreq (void) const
{
    return m_rx_freq_MHz;
}

void
LoRaPHY::SetTxSF (uint8_t sf)
{
    if (sf >= MINIMUM_LORA_SPREADING_FACTOR && sf <= MAXIMUM_LORA_SPREADING_FACTOR)
    {
        m_tx_sf = sf;
    }

    return;
}

uint8_t
LoRaPHY::GetTxSF (void) const
{
    return m_tx_sf;
}

void
LoRaPHY::SetRxSF (uint8_t sf)
{
    if (sf >= MINIMUM_LORA_SPREADING_FACTOR && sf <= MAXIMUM_LORA_SPREADING_FACTOR)
    {
        m_rx_sf = sf;
    }

    return;
}

uint8_t
LoRaPHY::GetRxSF (void) const
{
    return m_rx_sf;
}

void
LoRaPHY::SetState (PHYState new_state)
{
    m_state = new_state;
}

PHYState
LoRaPHY::GetState (void) const
{
    return m_state;
}

void
LoRaPHY::Send (Ptr<Packet> packet)
{
    if (m_state != STANDBY)
    {
        return; /*  phy layer busy  */
    }
    
    SetState (TX);
    
    m_channel->Send (this, packet, m_tx_power_dBm, m_tx_freq_MHz, m_tx_sf);
    
    //wait for tx to finish implement callback
    
    return;
}

void
LoRaPHY::Receive (Ptr<Packet> packet)
{
    if (m_state != STANDBY)
    {
        return;
    }
}

}
}
