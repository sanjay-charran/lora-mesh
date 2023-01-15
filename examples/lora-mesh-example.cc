/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/lora-mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "mobility-helper.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-mac.h"
#include "ns3/lora-net-device.h"
#include "ns3/lora-channel.h"

#define NUM_NODES   4

using namespace ns3;
using namespace lora_mesh;

NS_LOG_COMPONENT_DEFINE ("LoRaMeshExample");

int 
main(int argc, char *argv[])
{   
    //create channel
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent (3.76);
    loss->SetReference (1, 7.7);
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    
    channel->SetLossModel(loss);
    channel->SetDelayModel(delay);
    
    //setup mobility and initial positions
    MobilityHelper mobility;
    
    Ptr<RandomDiscPositionAllocator> position = CreateObject<RandomDiscPositionAllocator>();    //set rho and theta with connect
    
    mobility.SetPositionAllocator(position);
    mobility.SetMobilityModel("ns3::RandomWaypointMobilityModel");  //set position allocator for this
    
    
    //create nodes
    NodeContainer loranodes;
    loranodes.Create(NUM_NODES);
    
    mobility.Install(loranodes);
    
    //create phys for nodes
    //create macs for nodes
    //create devices for nodes
    //attach devices, phys and macs
    //add phys to channel
    
    for (NodeContainer::Iterator i = loranodes.begin(); i != loranodes.end(); ++i)
    {
        Ptr<Node> node = *i;
        Ptr<LoRaNetDevice> device = CreateObject<LoRaNetDevice>();
        Ptr<LoRaPHY> phy = CreateObject<LoRaPHY>();
        Ptr<LoRaMAC> mac = CreateObject<LoRaMAC>();
        
        
        phy->SetChannel(channel);
        phy->SetNetDevice(device);
        phy->SetMAC(mac);
        //phy tx/rx params
        
        mac->SetPHY(phy);
        mac->SetDevice(device);
        
        channel->AddPHY(phy);
        
        device->SetMAC(mac);
        device->SetPHY(phy);
        device->SetNode(node);
        //device params
        
        node->AddDevice(device);
    }
    
    //install applications
    //simulator setups
    

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}


