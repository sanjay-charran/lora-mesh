/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/simulator.h"

#include "ns3/lora-phy.h"

namespace ns3 {
namespace lora_mesh {

TypeId
LoRaPHY::GetTypeId (void)
{
    static tid = TypeId ("ns3::LoRaPHY")
        .SetParent<Object> ()
        .SetGroupName ("lora_mesh")
        .AddTraceSource ("StartSending",
                        "Trace source indicating the PHY layer"
                        "has begun the sending process for a packet",
                        MakeTraceSourceAccessor (&LoraPhy::m_startSending),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("PhyRxBegin",
                        "Trace source indicating a packet "
                        "is now being received from the channel medium "
                        "by the device",
                        MakeTraceSourceAccessor (&LoraPhy::m_phyRxBeginTrace),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("PhyRxEnd",
                        "Trace source indicating the PHY has finished "
                        "the reception process for a packet",
                        MakeTraceSourceAccessor (&LoraPhy::m_phyRxEndTrace),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("ReceivedPacket",
                        "Trace source indicating a packet "
                        "was correctly received",
                        MakeTraceSourceAccessor
                        (&LoraPhy::m_successfullyReceivedPacket),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("LostPacketBecauseInterference",
                        "Trace source indicating a packet "
                        "could not be correctly decoded because of interfering"
                        "signals",
                        MakeTraceSourceAccessor (&LoraPhy::m_interferedPacket),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("LostPacketBecauseUnderSensitivity",
                        "Trace source indicating a packet "
                        "could not be correctly received because"
                        "its received power is below the sensitivity of the receiver",
                        MakeTraceSourceAccessor (&LoraPhy::m_underSensitivity),
                        "ns3::Packet::TracedCallback");
        
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
LoRaPHY::SetChannel (Ptr<LoRaChannel> c)
{
    m_channel = c;
    return;
}

Ptr<LoRaChannel> 
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
LoRaPHY::SwitchStateTX (void)
{
    m_state = TX;
    return;
}

void
LoRaPHY::SwitchStateRX (void)
{
    m_state = RX;
    return;
}

void
LoRaPHY::SwitchStateSTANDBY (void)
{
    m_state = STANDBY;
    return;
}

void
LoRaPHY::SwitchStateSLEEP (void)
{
    m_state = SLEEP;
    return;
}

PHYState
LoRaPHY::GetState (void) const
{
    return m_state;
}

void
LoRaPHY::SetTxBW (double bw_Hz)
{
    m_tx_bandwidth_Hz = bw_Hz;
    return;
}

double
LoRaPHY::GetTxBW (void) const
{
    return m_tx_bandwidth_Hz;
}

void
LoRaPHY::SetTxPreamples (uint32_t num_preamples)
{
    m_tx_numPreambles = num_preamples;
    return;
}

uint32_t
LoRaPHY::GetTxPreamples (void) const
{
    return m_tx_numPreambles;
}

void
LoRaPHY::SetTxCodingRate (uint8_t coding_rate)
{
    m_tx_codingRate = coding_rate;
    return;
}

uint8_t
LoRaPHY::GetCodingRate (void) const
{
    return m_tx_codingRate;
}

void
LoRaPHY::ToggleHeader (void)
{
    m_tx_headerDisabled = !m_tx_headerDisabled;
    return;
}

bool
LoRaPHY::IsHeaderDisabled (void) const
{
    return m_tx_headerDisabled;
}

void
LoRaPHY::ToggleCRC (void)
{
    m_crcEnabled = !m_crcEnabled;
    return;
}

bool
LoRaPHY::IsCRCEnabled (void) const
{
    return m_crcEnabled;
}

void
LoRaPHY::ToggleLowDataRateOpt (void)
{
    m_lowDataRateOpt = !m_lowDataRateOpt;
    return;
}

bool IsLowDataRateOptEnabled (void) const
{
    return m_lowDataRateOpt;
}

void
LoRaPHY::Send (Ptr<Packet> packet)
{
    if (m_state != STANDBY)
    {
        return; /*  phy layer busy  */
    }
    
    Time dur = GetOnAirTime (packet);
    
    SwitchStateTX ();
    
    //not sure about tag bit need to dbl check
    
    m_channel->Send (this, packet, m_tx_power_dBm, m_tx_freq_MHz, m_tx_sf, dur);
    
    Simulator::Scedule (dur, &SwitchStateSTANDBY, this);
    
    if (m_device)
    {
        m_startSending (packet, m_device->GetNode()->GetId());
    }
    else
    {
        m_startSending (packet, 0);
    }
    
    return;
}

Time
LoRaPHY::GetOnAirTime (Ptr<Packet> packet)
{
    double Ts = pow(2, m_tx_sf) / m_tx_bandwidth_Hz;    /*  in seconds  */
    double Tpreamble = (m_tx_numPreambles + 4.25) * Ts;
    uint32_t pl = packet->GetSize ();
    
    double de = m_lowDataRateOpt ? 1 : 0;
    double h = m_tx_headerDisabled? 1 : 0;
    double crc = m_crcEnabled? 1 : 0;
    
    double payloadSymNb = 8 + std::max((std::ceil((8*pl - 4*m_tx_sf + 28 + 16*crc - 20*h) / (4*(m_tx_sf - 2*de)))) *
                            (m_tx_codingRate + 4), (double)0.0);
    
    double Tpayload = payloadSymNb * Ts;
    
    return Seconds(Tpayload + Tpreamble);
}

void
LoRaPHY::StartReceive (Ptr<Packet> packet, Time duration, uint8_t sf, double rx_power_dBm, double freq_MHz)
{
    bool canLockOn = true;
    
    if (m_state == STANDBY)
    {
        if (freq_MHz != m_rx_freq_MHz)
        {
            canLockOn = false;
        }
        
        if (sf != m_tx_sf)
        {
            canLockOn = false;
        }
        
        if (rx_power_dBm < m_rx_sens_dBm)
        {
            canLockOn = false;
        }
        
        if (canLockOn)
        {
            SwitchStateRX();
            
            Simulator::Scedule (duration, &LoRaPHY::EndReceive, this, packet);
            
            m_phyRxBeginTrace (packet);
        }
    }
    
    return;
}

void
LoRaPHY::EndReceive (Ptr<packet> packet)
{
    SwitchStateSTANDBY ();
    
    m_phyRxEndTrace (packet);
    
    if (m_device)
    {
        m_successfullyReceivedPacket (packet, m_device->GetNode()->GetId());
    }
    else
    {
        m_successfullyReceivedPacket (packet, 0);
    }
}

}
}
