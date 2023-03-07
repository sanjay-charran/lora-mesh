/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/lora-mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/application.h"
#include "ns3/mobility-helper.h"
#include "ns3/application-container.h"
#include "ns3/random-waypoint-mobility-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/mobility-helper.h"
#include "ns3/node-container.h"
#include "ns3/ptr.h"
#include "ns3/net-device-container.h"
#include "ns3/object-factory.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-mac.h"
#include "ns3/lora-net-device.h"
#include "ns3/lora-channel.h"

#include <iterator>
#include <vector>

#define NUM_NODES   4

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
    MobilityHelper mobility;
    
    Ptr<RandomDiscPositionAllocator> position = CreateObject<RandomDiscPositionAllocator>();
    
    mobility.SetPositionAllocator(position);
    mobility.SetMobilityModel(  "ns3::RandomWaypointMobilityModel",
                                "Speed", StringValue ("ns3::UniformRandomVariable[Min=1.34|Max=1.79]"),
                                "Pause", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"),
                                "PositionAllocator", PointerValue(position)
    );
    
    
    //create nodes
    NodeContainer loranodes;
    loranodes.Create(NUM_NODES);
    
    mobility.Install(loranodes);
    
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
        //phy tx/rx params (using def here)
        
        channel->AddPHY(phy);
        
        device->SetMAC(mac);
        device->SetPHY(phy);
        device->SetNode(node);
        
        node->AddDevice(device);
        //device params
        
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
    //simulator setups
    
    //NodeContainer::Iterator i = loranodes.Begin();
    Ptr<Packet> packet = Create<Packet>(50);
    LoRaMeshHeader header;
    
    header.SetType(DIRECTED);
    header.SetSrc(loranodes.Get(0)->GetId());
    header.SetFwd(loranodes.Get(0)->GetId());
    header.SetDest(loranodes.Get(3)->GetId());
    packet->AddHeader(header);
    
    loranodes.Get(0)->GetDevice(0)->Send(packet, Address(), 0);
    //loranodes.Get(0)->GetObject<LoRaNetDevice>()->SendTo(packet, 2);
    
    Simulator::Stop(Minutes(5));
    Simulator::Run ();
    Simulator::Destroy ();
    
    return 0;
}


