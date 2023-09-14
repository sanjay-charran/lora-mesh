/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/lora-mesh.h"
#include "ns3/lwb.h"

#include "ns3/ptr.h"
#include "ns3/core-module.h"
#include "ns3/test.h"
#include "ns3/node-container.h"
#include "ns3/vector.h"
#include "ns3/mobility-helper.h"
#include "ns3/simulator.h"
#include "ns3/callback.h"
#include "ns3/application.h"

using namespace ns3;
using namespace lora_mesh;

NS_LOG_COMPONENT_DEFINE("LWBTestSuite");

/************************************************************************************/
/*  Test Case #1: Glossy Test   */
class GlossyTestCase : public TestCase
{
public:
    GlossyTestCase();
    virtual ~GlossyTestCase();

private:
    virtual void DoRun(void);
};

GlossyTestCase::GlossyTestCase()
  : TestCase("LWB Test Case #1: Glossy")
{
}

GlossyTestCase::~GlossyTestCase()
{
}

void
GlossyTestCase::DoRun(void)
{
    NodeContainer       nodes;
    Ptr<LoRaPHY>        phy;
    Ptr<LWB>            mac;
    Ptr<LoRaNetDevice>  device;
    Ptr<LoRaChannel>    channel;
    Ptr<Glossy>         glossy;
    
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent(3.76);
    loss->SetReference(1, 7.7);
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel>();
    
    channel->SetLossModel(loss);
    channel->SetDelayModel(delay);
    
    MobilityHelper mobility;
    
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    
    nodes.Create(2);
    mobility.Install(nodes);
    
    for (NodeContainer::Iterator i = nodes.Begin();i != nodes.End();++i)
    {
        Ptr<Node> node = *i;
        device = Create<LoRaNetDevice>();
        phy = CreateObject<LoRaPHY>();
        mac = CreateObject<LWB>();
        glossy = CreateObject<Glossy>();
        
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
        mac->SetGlossy(glossy);
        glossy->SetLWB(mac);
        glossy->SetNode(node);
    }
    
    for (NodeContainer::Iterator i = nodes.Begin();i != nodes.End(); ++i)
    {
        Ptr<Application> app = CreateObject<Application>();
        
        app->SetNode(*i);
        (*i)->AddApplication(app);
    }
    
    Ptr<Packet> packet = Create<Packet>(RF_CONF_MAX_PKT_LEN);
    nodes.Get(0)->GetDevice(0)->Send(packet, Address(), 0);
    
    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/

class LWBTestSuite : public TestSuite
{
public:
    LWBTestSuite();
};

LWBTestSuite::LWBTestSuite()
    : TestSuite("lwb-test", UNIT)
{
    LogComponentEnable("LWBTetstSuite", LOG_LEVEL_ALL);
    
//     TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
    AddTestCase(new GlossyTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LWBTestSuite LWB_test_suite;
