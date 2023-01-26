#include "ns3/core-module.h"
#include "ns3/lora-mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/application.h"
#include "ns3/mobility-helper.h"
#include "ns3/application-container.h"
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
#include <vector>

using namespace ns3;
using namespace lora_mesh;

NS_LOG_COMPONENT_DEFINE ("LoRaMeshExample");

int
main (int argc, char *argv[])
{
    Time::SetResolution (Time::NS);
    LogComponentEnableAll(LOG_PREFIX_TIME);
    LogComponentEnable("LoRaMAC", LOG_LEVEL_INFO);
    LogComponentEnable("LoRaPHY", LOG_LEVEL_INFO);
    NS_LOG_UNCOND ("LoRa Mesh Simulation Example..." << std::endl);
    
    //create channel
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent (3.76);
    loss->SetReference (1, 7.7);
    
    channel->SetLossModel(loss);
    channel->SetDelayModel(delay);
    
    //mobility
    MobilityHelper mobility;
    
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    
    //setup nodes
    NodeContainer loranodes;
    loranodes.Create(21);
    
    mobility.Install(loranodes);
    
    
    NodeContainer nodes;
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
        
        nodes.Add(node);
    }
    
    //set locations
    Vector3D pos;
    pos.z = 0;
    
    pos.x = 0;
    pos.y = 174;
    loranodes.Get(4)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 19;
    pos.y = 136;
    loranodes.Get(1)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 288;
    pos.y = 343;
    loranodes.Get(15)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 300;
    pos.y = 12;
    loranodes.Get(18)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 306;
    pos.y = 374;
    loranodes.Get(3)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 319;
    pos.y = 562;
    loranodes.Get(19)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 350;
    pos.y = 30;
    loranodes.Get(17)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 376;
    pos.y = 112;
    loranodes.Get(8)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 388;
    pos.y = 150;
    loranodes.Get(7)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 388;
    pos.y = 530;
    loranodes.Get(16)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 438;
    pos.y = 424;
    loranodes.Get(12)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 476;
    pos.y = 450;
    loranodes.Get(5)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 476;
    pos.y = 406;
    loranodes.Get(14)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 526;
    pos.y = 443;
    loranodes.Get(13)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 612;
    pos.y = 430;
    loranodes.Get(9)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 650;
    pos.y = 274;
    loranodes.Get(10)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 676;
    pos.y = 456;
    loranodes.Get(11)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 788;
    pos.y = 500;
    loranodes.Get(2)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 788;
    pos.y = 530;
    loranodes.Get(0)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 600;
    pos.y = 456;
    loranodes.Get(20)->GetObject<MobilityModel>()->SetPosition(pos);
    
    //install applications
    
    ApplicationContainer apps;
    
    for (NodeContainer::Iterator i = loranodes.Begin();i != loranodes.End(); ++i)
    {
        Ptr<Application> app = CreateObject<Application>();
        
        app->SetNode(*i);
        (*i)->AddApplication(app);
        
        apps.Add(app);
    }
    
    //simulator setup
    Simulator::Stop(Minutes(2));
    Simulator::Run ();
    Simulator::Destroy ();
    
    return 0;
}
