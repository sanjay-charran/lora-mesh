/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/lora-mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/application.h"
#include "ns3/mobility-helper.h"
#include "ns3/application-container.h"
#include "ns3/random-waypoint-mobility-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/mobility-helper.h"
#include "ns3/node-container.h"
#include "ns3/ptr.h"
#include "ns3/net-device-container.h"
#include "ns3/object-factory.h"
#include "ns3/vector.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-mac.h"
#include "ns3/lora-net-device.h"
#include "ns3/lora-channel.h"

#include <iterator>

#define NUM_NODES       10
#define SIMULATION_SF   6

using namespace ns3;
using namespace lora_mesh;

NS_LOG_COMPONENT_DEFINE ("LoRaMeshExample");

int 
main(int argc, char *argv[])
{   
    Time::SetResolution (Time::NS);
    LogComponentEnableAll(LOG_PREFIX_TIME);
    LogComponentEnable("LoRaMAC", LOG_LEVEL_INFO);
    LogComponentEnable("LoRaPHY", LOG_LEVEL_INFO);
    NS_LOG_UNCOND ("LoRa Mesh Simulation Example...");
    
    //create channel
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent (2);
    loss->SetReference (1, 7.7);
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    
    channel->SetLossModel(loss);
    channel->SetDelayModel(delay);
    
    //setup mobility and initial positions
    MobilityHelper mobility, mobility_centre;
    
    Ptr<RandomDiscPositionAllocator> position = CreateObject<RandomDiscPositionAllocator>();
    
    mobility.SetPositionAllocator(position);
    mobility.SetMobilityModel(  "ns3::RandomWaypointMobilityModel",
                                "Speed", StringValue ("ns3::UniformRandomVariable[Min=1.34|Max=1.79]"),
                                "Pause", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"),
                                "PositionAllocator", PointerValue(position)
    );
    
    mobility_centre.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    
    //create nodes
    NodeContainer loranodes, centre;
    loranodes.Create(NUM_NODES);
    centre.Create(1);
    
    mobility.Install(loranodes);
    mobility_centre.Install(centre);
    
    //create phys for nodes
    //create macs for nodes
    //create devices for nodes
    //attach devices, phys and macs
    //add phys to channel
    
    Ptr<LoRaNetDevice> device;
    Ptr<LoRaPHY> phy;
    Ptr<LoRaMAC> mac;
    
    for (NodeContainer::Iterator i = loranodes.Begin(); i != loranodes.End(); ++i)
    {
        Ptr<Node> node = *i;
        device = Create<LoRaNetDevice>();
        phy = CreateObject<LoRaPHY>();
        mac = CreateObject<LoRaMAC>();
        
        phy->SetChannel(channel);
        phy->SetNetDevice(device);
        phy->SetMAC(mac);

        //sx1278
        phy->SetRxSens(-148); //dBm
        phy->SetTxPower(20);    //dBm
        phy->SetRxFreq(860);    //MHz
        phy->SetTxFreq(860);    //MHz
        phy->SetTxSF(SIMULATION_SF);    
        phy->SetRxSF(SIMULATION_SF);
        phy->SetTxBW(125000);   //Hz

        channel->AddPHY(phy);
        device->SetMAC(mac);
        device->SetPHY(phy);
        device->SetNode(node);
        node->AddDevice(device);
        mac->SetMinDelay(0);
        mac->SetMaxDelay(25);
        mac->SetPHY(phy);
        mac->SetDevice(device);
    }
    
    for (NodeContainer::Iterator i = centre.Begin(); i != centre.End(); ++i)
    {
        Ptr<Node> node = *i;
        device = Create<LoRaNetDevice>();
        phy = CreateObject<LoRaPHY>();
        mac = CreateObject<LoRaMAC>();
        
        phy->SetChannel(channel);
        phy->SetNetDevice(device);
        phy->SetMAC(mac);
        //sx1272
        phy->SetRxSens(-137); //dBm
        phy->SetTxPower(20);    //dBm
        phy->SetRxFreq(860);    //MHz
        phy->SetTxFreq(860);    //MHz
        phy->SetTxSF(SIMULATION_SF);    
        phy->SetRxSF(SIMULATION_SF);
        phy->SetTxBW(125000);   //Hz
        
        channel->AddPHY(phy);
        
        device->SetMAC(mac);
        device->SetPHY(phy);
        device->SetNode(node);
        
        node->AddDevice(device);
        //device params
        
        mac->SetMinDelay(0);
        mac->SetMaxDelay(5);
        mac->SetPHY(phy);
        mac->SetDevice(device);
    }
    
    //install applications
    ApplicationContainer apps;
    
    for (NodeContainer::Iterator i = loranodes.Begin();i != loranodes.End(); ++i)
    {
        Ptr<Application> app = CreateObject<Application>();
        
        app->SetNode(*i);
        (*i)->AddApplication(app);
        
        apps.Add(app);
    }
    
    for (NodeContainer::Iterator i = centre.Begin();i != centre.End(); ++i)
    {
        Ptr<Application> app = CreateObject<Application>();
        
        app->SetNode(*i);
        (*i)->AddApplication(app);
        
        apps.Add(app);
    }
    //simulator setups
    
    //fix location of gateway node
    Vector3D pos;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    centre.Get(0)->GetObject<MobilityModel>()->SetPosition(pos);
    
    AsciiHelperForLoRa ascii;
    ascii.EnableAscii("AnimalTracking", loranodes);
    ascii.EnableAscii("AnimalTracking", centre);
    
    for (NodeContainer::Iterator i = loranodes.Begin();i != loranodes.End(); ++i)
    {
        Ptr<Node> node = *i;
        
        for (unsigned int j = 0;j < 10;j++)
        {
            Ptr<Packet> packet = Create<Packet>(50);
            LoRaMeshHeader header;
        
            header.SetType(DIRECTED);
            header.SetSrc(node->GetId());
            header.SetFwd(node->GetId());
            header.SetDest(centre.Get(0)->GetId());
            packet->AddHeader(header);
        
            node->GetDevice(0)->Send(packet, Address(), 0);
        }
    }
    
    Simulator::Stop(Minutes(40));
    Simulator::Run ();
    Simulator::Destroy ();
    
    return 0;
}


