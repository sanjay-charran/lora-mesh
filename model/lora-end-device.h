/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __LORA_END_DEVICE_H__
#define __LORA_END_DEVICE_H__

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/lora-mesh-channel.h"   //to be written
#include <vector>

namespace ns3 {
namespace lora_mesh {
    
struct RoutingTableEntry
{
    uint16_t    s;
    uint16_t    r;
    float       etx;
    uint8_t     last;    
};
    
class LoRaEndDevice : public Object
{
pubilc:

    /*  Constructor & Destructor    */
    LoRaEndDevice ();
    ~LoRaEndDevice ();
    
    /*  TypeId  */
    static TypeId GetTypeId (void);
    
    /*  setter and getter for id    */
    void SetID (uint16_t id);
    uint16_t GetID (void);
    
    /*  setter and getter for net device    */
    void SetNetDevice (Ptr<NetDevice> nd);
    Ptr<NetDevice> GetNetDevice (void) const;
    
    /*  setter and getter for channel   */
    void SetChannel (Ptr<LoRaMeshChannel> c);
    Ptr<LoRaMeshChannel> GetChannel (void) const;
    
    /*  add and remove entry for routing table  */
    void AddTableEntry (RoutingTableEntry entry);
    RoutingTableEntry RemoveTableEntry (utin64_t n);        // adjsut lookup vector length max based on max id
    
    /*  routing table lookup    */
    RoutingTableEntry TableLookup (uint64_t n) const;
    
    /*  sending and receiving */
//     void Send ();       //add in parameters for these
//     void Receive ();    //
    
private:
    /*  unique device id    */
    uint16_t m_id;              // need to define max id
    
    /*  the net device and channel attached to this node    */
    Ptr<NetDevice> m_device;
    Ptr<LoRaMeshChannel> m_channel;
    
    /*  transmit power and receiver sensitivity of this device  */
    double m_tx_power_dBm;
    double m_rx_sens_dBm;
    
    /*  transmit frequency and spreading factor */
    double m_tx_freq_MHz;
    uint8_t m_tx_sf;
    
    /*  frequency and spreading factor to listen for as receiver    */
    double m_rx_freq_MHz;
    uint8_t m_rx_sf;
    
    /*  routing table containing info the end device is aware of    */
    vector<RoutingTableEntry> m_table;
};
    
}
}

#endif /*   __LORA_END_DEVICE_H__    */
