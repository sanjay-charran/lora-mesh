/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __LORA_PHY_H__
#define __LORA_PHY_H__

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/nstime.h

#include "ns3/lora-mesh-channel.h"   //to be written
#include "ns3/lora-net-device.h"

#define MINIMUM_LORA_SPREADING_FACTOR   7
#define MAXIMUM_LORA_SPREADING_FACTOR   12

namespace ns3 {
namespace lora_mesh {
    
enum PHYState
{
    SLEEP,
    TX,
    RX,
    STANDBY
};
    
class LoRaPHY : public Object
{
pubilc:

    /*  Constructor & Destructor    */
    LoRaPHY ();
    ~LoRaPHY ();
    
    /*  TypeId  */
    static TypeId GetTypeId (void);
    
    /*  setter and getter for net device    */    // add setters and getters for tx/rx params
    void SetNetDevice (Ptr<LoRaNetDevice> lnd);
    Ptr<LoRaNetDevice> GetNetDevice (void) const;
    
    /*  setter and getter for channel   */
    void SetChannel (Ptr<LoRaMeshChannel> c);
    Ptr<LoRaMeshChannel> GetChannel (void) const;
    
    /*  setter and getters for tx/rx params */
    void SetTxPower (double power_dBm);
    double GetTxPower (void) const;
    
    void SetTxFreq (double freq_MHz);
    double GetTxFreq (void) const;
    
    void SetTxSF (uint8_t sf);
    uint8_t GetTxSF (void) const;
    
    void SetTxBW (double bw_Hz);//
    double GetTxBW (void) const;//
    
    void SetTxPreamples (uint32_t num_preamples);//
    uint32_t GetTxPreamples (void) const;//
    
    void SetRxSens (double sens_dBm);
    double GetRxSens (void) const;
    
    void SetRxFreq (double freq_MHz);
    double GetRxFreq (void) const;
    
    void SetRxSF (uint8_t sf);
    uint8_t GetRxSF (void) const;
    
    void SwitchStateSLEEP (void);
    void SwitchStateSTANDBY (void);
    PHYState GetState (void) const;
    
    /*  sending and receiving */
    void Send (Ptr<Packet> packet); /*  send/receive using current transmit/receive parameters  */
    void Receive (Ptr<Packet> packet);//
    
    Time GetOnAirTime (Ptr<Packet> packet);
    
protected:
    void SwitchStateTX (void);
    void SwitchStateRX (void);
    
    PHYState m_state;
    
    /*  the net device and channel attached to this node    */
    Ptr<LoRaNetDevice> m_device;
    Ptr<LoRaMeshChannel> m_channel;
    
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
    double m_rx_sens_dBm;
    double m_rx_freq_MHz;
    uint8_t m_rx_sf;

    TracedCallback<Ptr<const Packet>, uint32_t> m_startSending;
    TracedCallback<Ptr<const Packet> > m_phyRxBeginTrace;
    TracedCallback<Ptr<const Packet> > m_phyRxEndTrace;
    TracedCallback<Ptr<const Packet>, uint32_t> m_successfullyReceivedPacket;
    TracedCallback<Ptr<const Packet>, uint32_t> m_underSensitivity;
    TracedCallback<Ptr<const Packet>, uint32_t> m_interferedPacket;
};
    
}
}
#endif /*   __LORA_PHY_H__    */
