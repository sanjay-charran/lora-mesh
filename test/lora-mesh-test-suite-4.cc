/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/lora-mesh.h"

#include "ns3/ptr.h"
#include "ns3/core-module.h"
#include "ns3/test.h"
#include "ns3/node-container.h"
#include "ns3/vector.h"
#include "ns3/mobility-helper.h"
#include "ns3/simulator.h"
#include "ns3/callback.h"
#include "ns3/application.h"

#include <iterator>

using namespace ns3;
using namespace lora_mesh;

NS_LOG_COMPONENT_DEFINE("LoRaMeshTestSuite_4");

/************************************************************************************/
/*  Test Case #4.1: Packet Collisions   */
class LoRaMeshTestCase4_1 : public TestCase
{
public:
    LoRaMeshTestCase4_1();
    virtual ~LoRaMeshTestCase4_1();

private:
    virtual void DoRun(void);
};

LoRaMeshTestCase4_1::LoRaMeshTestCase4_1()
  : TestCase("LoRa Mesh Test Case #4.1: Packet Collisions")
{
}

LoRaMeshTestCase4_1::~LoRaMeshTestCase4_1()
{
}

void
LoRaMeshTestCase4_1::DoRun(void)
{
    NodeContainer nodes;
    Ptr<LoRaPHY> phy;
    Ptr<LoRaMAC> mac;
    Ptr<LoRaNetDevice> device;
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent(3.76);
    loss->SetReference(1, 7.7);
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel>();
    
    channel->SetLossModel(loss);
    channel->SetDelayModel(delay);
    
    MobilityHelper mobility;
    
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    
    nodes.Create(3);
    mobility.Install(nodes);
    
    for (NodeContainer::Iterator i = nodes.Begin();i != nodes.End();++i)
    {
        Ptr<Node> node = *i;
        device = Create<LoRaNetDevice>();
        phy = CreateObject<LoRaPHY>();
        mac = CreateObject<LoRaMAC>();
        
        mac->SetMinDelay(0);
        mac->SetMaxDelay(2);
        
        phy->SetNetDevice(device);
        phy->SetMAC(mac);
        phy->SetChannel(channel);
        channel->AddPHY(phy);
        device->SetMAC(mac);
        device->SetPHY(phy);
        device->SetNode(node);
        node->AddDevice(device);
        mac->SetPHY(phy);
        mac->SetDevice(device);
    }
    
    for (NodeContainer::Iterator i = nodes.Begin();i != nodes.End(); ++i)
    {
        Ptr<Application> app = CreateObject<Application>();
        
        app->SetNode(*i);
        (*i)->AddApplication(app);
    }
    
    Vector3D pos;
    
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    nodes.Get(0)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 1;
    pos.y = 0;
    pos.z = 0;
    nodes.Get(1)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 0;
    pos.y = 1;
    pos.z = 0;
    nodes.Get(2)->GetObject<MobilityModel>()->SetPosition(pos);
    
    /*  make packet to send */
    Ptr<Packet> packet = Create<Packet>(50);
    LoRaMeshHeader header;
    header.SetType(DIRECTED);
    header.SetSrc(nodes.Get(0)->GetId());
    header.SetFwd(nodes.Get(0)->GetId());
    header.SetDest(nodes.Get(1)->GetId());
    packet->AddHeader(header);
    
    Ptr<Packet> packet2 = Create<Packet>(50);
    LoRaMeshHeader header2;
    header2.SetType(DIRECTED);
    header2.SetSrc(nodes.Get(2)->GetId());
    header2.SetFwd(nodes.Get(2)->GetId());
    header2.SetDest(nodes.Get(1)->GetId());
    packet2->AddHeader(header);
    
    nodes.Get(0)->GetDevice(0)->Send(packet, Address(), 0);
    nodes.Get(2)->GetDevice(0)->Send(packet2, Address(), 0);
    
    
    Simulator::Stop(Minutes(1));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/

class LoRaMeshTestSuite_4 : public TestSuite
{
public:
    LoRaMeshTestSuite_4();
};

LoRaMeshTestSuite_4::LoRaMeshTestSuite_4()
    : TestSuite("lora-mesh-4", UNIT)
{
    LogComponentEnable("LoRaMeshTestSuite_4", LOG_LEVEL_ALL);
    LogComponentEnable("LoRaPHY", LOG_LEVEL_ALL);
    LogComponentEnable("LoRaMAC", LOG_LEVEL_ALL);
    LogComponentEnable("LoRaChannel", LOG_LEVEL_ALL);
    
    // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
    AddTestCase(new LoRaMeshTestCase4_1, TestCase::TAKES_FOREVER);
}

// Do not forget to allocate an instance of this TestSuite
static LoRaMeshTestSuite_4 loraMeshTestSuite_4;
