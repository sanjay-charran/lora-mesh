/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/simulator.h"
#include "ns3/log.h"

#include "ns3/lora-phy.h"

NS_LOG_COMPONENT_DEFINE ("LoRaPHY");

namespace ns3 {
namespace lora_mesh {

TypeId
LoRaPHY::GetTypeId (void)
{
    static TypeId tid =  TypeId ("ns3::LoRaPHY")
        .SetParent<Object> ()
        .SetGroupName ("lora_mesh")
        .AddTraceSource ("StartSending",
                        "Trace source indicating the PHY layer"
                        "has begun the sending process for a packet",
                        MakeTraceSourceAccessor (&LoRaPHY::m_startSending),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("PhyRxBegin",
                        "Trace source indicating a packet "
                        "is now being received from the channel medium "
                        "by the device",
                        MakeTraceSourceAccessor (&LoRaPHY::m_phyRxBeginTrace),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("PhyRxEnd",
                        "Trace source indicating the PHY has finished "
                        "the reception process for a packet",
                        MakeTraceSourceAccessor (&LoRaPHY::m_phyRxEndTrace),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("ReceivedPacket",
                        "Trace source indicating a packet "
                        "was correctly received",
                        MakeTraceSourceAccessor(&LoRaPHY::m_successfullyReceivedPacket),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("LostPacketBecauseInterference",
                        "Trace source indicating a packet "
                        "could not be correctly decoded because of interfering"
                        "signals",
                        MakeTraceSourceAccessor (&LoRaPHY::m_interferedPacket),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("LostPacketBecauseUnderSensitivity",
                        "Trace source indicating a packet "
                        "could not be correctly received because"
                        "its received power is below the sensitivity of the receiver",
                        MakeTraceSourceAccessor (&LoRaPHY::m_underSensitivity),
                        "ns3::Packet::TracedCallback")
        .AddTraceSource ("RxSniffer",
                        "Trace source which simulates sniffing for received packets",
                        MakeTraceSourceAccessor (&LoRaPHY::m_rxSniffer),
                        "ns3::LoRaPHY::RxSnifferTracedCallback")
        .AddTraceSource ("TxSniffer",
                        "Trace source which simulates sniffing for transmitted packets",
                        MakeTraceSourceAccessor (&LoRaPHY::m_txSniffer),
                        "ns3::LoRaPHY::TxSnifferTracedCallback");
        
    return tid;
}
    
LoRaPHY::LoRaPHY ()
{
    m_packet_collision = false;
    m_state = STANDBY;
    
    m_tx_headerDisabled = false;
    m_tx_codingRate = 1;
    m_tx_bandwidth_Hz = 125000;
    m_tx_numPreambles = 8;
    m_crcEnabled = true;
    m_lowDataRateOpt = false;
    
    m_tx_sf = 7;
    m_rx_sf = 7;
    m_tx_power_dBm = 14;
    m_rx_sens_dBm = -124;
    m_rx_freq_MHz = 868.1;
    m_tx_freq_MHz = 868.1;
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
LoRaPHY::SetMAC (Ptr<LoRaMAC> mac)
{
    m_mac = mac;
    return;
}

Ptr<LoRaMAC> 
LoRaPHY::GetMAC (void) const
{
    return m_mac;
}

void
LoRaPHY::SetMobility (Ptr<MobilityModel> mobility)
{
    m_mobility = mobility;
    return;
}

Ptr<MobilityModel> 
LoRaPHY::GetMobility (void) const
{
    if (m_mobility != 0)
    {
        return m_mobility;
    }
    else
    {
        return m_device->GetNode()->GetObject<MobilityModel>();
    }
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

bool 
LoRaPHY::IsLowDataRateOptEnabled (void) const
{
    return m_lowDataRateOpt;
}

void
LoRaPHY::Send (Ptr<Packet> packet)
{
    NS_LOG_FUNCTION (this << packet);
    
    if (m_state != STANDBY)
    {
        return; /*  phy layer busy  */
    }
    
    Time dur = GetOnAirTime (packet);
    
    SwitchStateTX ();
    
    //not sure about tag bit need to dbl check
    
    m_channel->Send (this, packet, m_tx_power_dBm, m_tx_freq_MHz, m_tx_sf, dur);
    
    Simulator::Schedule (dur, &LoRaPHY::SwitchStateSTANDBY, this);
    
    if (m_device)
    {
        //m_startSending (packet, m_device->GetNode()->GetId());
    }
    else
    {
        //m_startSending (packet, 0);
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
    
    if (m_state != SLEEP && m_state != TX)
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
            if (m_state == RX)
            {
                m_packet_collision = true;  // need to set to prevent the EndReceive call from being wrong
                
                NS_LOG_INFO("Packet Collision for Packet #" << packet->GetUid());
                
                if (Simulator::GetDelayLeft(m_last_receive_event).GetSeconds() < duration)
                {
                    Simulator::Cancel(m_last_receive_event);
                }
                
                //any packet collision handling will be called from here
            }
            
            SwitchStateRX();
            
            m_last_receive_event = Simulator::Schedule (duration, &LoRaPHY::EndReceive, this, packet);
            
            //m_phyRxBeginTrace (packet);
        }
    }
    
    return;
}

void
LoRaPHY::EndReceive (Ptr<Packet> packet)
{
    NS_LOG_FUNCTION (this << packet);
    
    SwitchStateSTANDBY ();
    
    //m_phyRxEndTrace (packet);
    
    if (m_mac && !m_packet_collision)
    {
        m_mac->Receive(packet);
    }
    
    m_packet_collision = false;
    
    if (m_device)
    {
        //m_successfullyReceivedPacket (packet, m_device->GetNode()->GetId());
    }
    else
    {
        //m_successfullyReceivedPacket (packet, 0);
    }
}

}
}
