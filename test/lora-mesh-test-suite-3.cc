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

NS_LOG_COMPONENT_DEFINE ("LoRaMeshTestSuite_3");

/************************************************************************************/
/*  Test Case #3.1: Routing Table Info  */
class LoRaMeshTestCase3_1 : public TestCase
{
public:
    LoRaMeshTestCase3_1 ();
    virtual ~LoRaMeshTestCase3_1 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase3_1::LoRaMeshTestCase3_1 ()
  : TestCase ("LoRa Mesh Test Case #3.1: Routing Table Info")
{
}

LoRaMeshTestCase3_1::~LoRaMeshTestCase3_1 ()
{
}

void
LoRaMeshTestCase3_1::DoRun (void)
{
    NodeContainer nodes;
    Ptr<LoRaPHY> phy;
    Ptr<LoRaMAC> mac;
    Ptr<LoRaNetDevice> device;
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent (3.76);
    loss->SetReference (1, 7.7);
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    
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
        mac = CreateObject<LoRaMAC>();
        
        mac->SetMinDelay(0);
        mac->SetMaxDelaay(5);
        
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
    
    
    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/
/*  Test Case #3.3: Multihop Routing Updates  */
class LoRaMeshTestCase3_3 : public TestCase
{
public:
    LoRaMeshTestCase3_3 ();
    virtual ~LoRaMeshTestCase3_3 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase3_3::LoRaMeshTestCase3_3 ()
  : TestCase ("LoRa Mesh Test Case #3.3: Multihop Routing Updates")
{
}

LoRaMeshTestCase3_3::~LoRaMeshTestCase3_3 ()
{
}

void
LoRaMeshTestCase3_3::DoRun (void)
{
    NodeContainer nodes;
    Ptr<LoRaPHY> phy;
    Ptr<LoRaMAC> mac;
    Ptr<LoRaNetDevice> device;
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent (3.76);
    loss->SetReference (1, 7.7);
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    
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
        mac->SetMaxDelaay(5);
        
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
    
    pos.x = 100;
    pos.y = 0;
    pos.z = 0;
    nodes.Get(1)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 0;
    pos.y = 100;
    pos.z = 0;
    nodes.Get(2)->GetObject<MobilityModel>()->SetPosition(pos);
    
    
    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/
/*  Test Case #3.4: Multihop Routing Updates  */
class LoRaMeshTestCase3_4 : public TestCase
{
public:
    LoRaMeshTestCase3_4 ();
    virtual ~LoRaMeshTestCase3_4 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase3_4::LoRaMeshTestCase3_4 ()
  : TestCase ("LoRa Mesh Test Case #3.4: Multihop Routing Updates")
{
}

LoRaMeshTestCase3_4::~LoRaMeshTestCase3_4 ()
{
}

void
LoRaMeshTestCase3_4::DoRun (void)
{
    NodeContainer nodes;
    Ptr<LoRaPHY> phy;
    Ptr<LoRaMAC> mac;
    Ptr<LoRaNetDevice> device;
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent (3.76);
    loss->SetReference (1, 7.7);
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    
    channel->SetLossModel(loss);
    channel->SetDelayModel(delay);
    
    MobilityHelper mobility;
    
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    
    nodes.Create(4);
    mobility.Install(nodes);
    
    for (NodeContainer::Iterator i = nodes.Begin();i != nodes.End();++i)
    {
        Ptr<Node> node = *i;
        device = Create<LoRaNetDevice>();
        phy = CreateObject<LoRaPHY>();
        mac = CreateObject<LoRaMAC>();
        
        mac->SetMinDelay(0);
        mac->SetMaxDelaay(10);
        
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
    
    pos.x = 50;
    pos.y = 20;
    pos.z = 0;
    nodes.Get(1)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = -50;
    pos.y = 20;
    pos.z = 0;
    nodes.Get(2)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 0;
    pos.y = 20;
    pos.z = 0;
    nodes.Get(3)->GetObject<MobilityModel>()->SetPosition(pos);
    
    Ptr<Packet> packet = Create<Packet>(50);
    LoRaMeshHeader header;
    header.SetType(DIRECTED);
    header.SetSrc(nodes.Get(1)->GetId());
    header.SetFwd(nodes.Get(1)->GetId());
    header.SetDest(nodes.Get(2)->GetId());
    packet->AddHeader(header);
    
    nodes.Get(1)->GetDevice(0)->Send(packet, Address(), 0);
    
    Simulator::Stop(Minutes(3));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/
/*  Test Case #3.6: Adjusting min/max value for delay RNG for node  */
class LoRaMeshTestCase3_6 : public TestCase
{
public:
    LoRaMeshTestCase3_6 ();
    virtual ~LoRaMeshTestCase3_6 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase3_6::LoRaMeshTestCase3_6 ()
  : TestCase ("LoRa Mesh Test Case #3.6: Adjusting min/max value for delay RNG for node")
{
}

LoRaMeshTestCase3_6::~LoRaMeshTestCase3_6 ()
{
}

void
LoRaMeshTestCase3_6::DoRun (void)
{
    double old_min, old_max, new_min, new_max;
    
    NodeContainer nodes;
    Ptr<LoRaPHY> phy;
    Ptr<LoRaMAC> mac;
    Ptr<LoRaNetDevice> device;
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent (3.76);
    loss->SetReference (1, 7.7);
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    
    channel->SetLossModel(loss);
    channel->SetDelayModel(delay);
    
    MobilityHelper mobility;
    
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    
    nodes.Create(1);
    mobility.Install(nodes);
    
    old_min = mac->GetMinDelay();
    old_max = mac->GetMaxDelay();
    
    for (NodeContainer::Iterator i = nodes.Begin();i != nodes.End();++i)
    {
        Ptr<Node> node = *i;
        device = Create<LoRaNetDevice>();
        phy = CreateObject<LoRaPHY>();
        mac = CreateObject<LoRaMAC>();
        
        mac->SetMinDelay(1);
        mac->SetMaxDelaay(10);
        
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
    
    NS_TEST_ASSERT_MSG_NE(mac->GetMinDelay(), old_min, "Test Case #3.6: Failed to set min Delay");
    NS_TEST_ASSERT_MSG_NE(mac->GetMaxDelay(), old_max, "Test Case #3.6: Failed to set max Delay");
    
    return;
}

/************************************************************************************/
/*  Test Case #3.7: Out of Range for Transmission   */
class LoRaMeshTestCase3_7 : public TestCase
{
public:
    LoRaMeshTestCase3_7 ();
    virtual ~LoRaMeshTestCase3_7 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase3_7::LoRaMeshTestCase3_7 ()
  : TestCase ("LoRa Mesh Test Case #3.7: Out of Range for Transmission")
{
}

LoRaMeshTestCase3_7::~LoRaMeshTestCase3_7 ()
{
}

void
LoRaMeshTestCase3_7::DoRun (void)
{
    NodeContainer nodes;
    Ptr<LoRaPHY> phy;
    Ptr<LoRaMAC> mac;
    Ptr<LoRaNetDevice> device;
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent (3.76);
    loss->SetReference (1, 7.7);
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    
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
        mac->SetMaxDelaay(10);
        
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
    
    pos.x = 100;
    pos.y = 0;
    pos.z = 0;
    nodes.Get(1)->GetObject<MobilityModel>()->SetPosition(pos);
    
    pos.x = 10000;
    pos.y = 0;
    pos.z = 0;
    nodes.Get(2)->GetObject<MobilityModel>()->SetPosition(pos);
    
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
    packet2->AddHeader(header2);
    
    nodes.Get(0)->GetDevice(0)->Send(packet, Address(), 0);
    nodes.Get(2)->GetDevice(0)->Send(packet, Address(), 0);
    
    Simulator::Stop(Minutes(3));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/


class LoRaMeshTestSuite_3 : public TestSuite
{
public:
    LoRaMeshTestSuite_3 ();
};

LoRaMeshTestSuite_3::LoRaMeshTestSuite_3 ()
    : TestSuite ("lora-mesh-3", UNIT)
{
    Time::SetResolution (Time::NS);
    LogComponentEnableAll(LOG_PREFIX_TIME);
    LogComponentEnable("LoRaMeshTestSuite_3", LOG_LEVEL_ALL);
    LogComponentEnable("LoRaPHY", LOG_LEVEL_ALL);
    LogComponentEnable("LoRaMAC", LOG_LEVEL_ALL);
    LogComponentEnable("LoRaChannel", LOG_LEVEL_ALL);
    
    // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
    AddTestCase (new LoRaMeshTestCase3_1, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase3_3, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase3_4, TestCase::EXTENSIVE);
    AddTestCase (new LoRaMeshTestCase3_6, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase3_7, TestCase::EXTENSIVE);
}

// Do not forget to allocate an instance of this TestSuite
static LoRaMeshTestSuite_3 loraMeshTestSuite_3;
