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

NS_LOG_COMPONENT_DEFINE ("LoRaMeshTestSuite_5");

/************************************************************************************/
/*  Test Case #5.1: Logging of Packet Sending   */
class LoRaMeshTestCase5_1 : public TestCase
{
public:
    LoRaMeshTestCase5_1 ();
    virtual ~LoRaMeshTestCase5_1 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase5_1::LoRaMeshTestCase5_1 ()
  : TestCase ("LoRa Mesh Test Case #5.1: Logging of Packet Sending")
{
}

LoRaMeshTestCase5_1::~LoRaMeshTestCase5_1 ()
{
}

void
LoRaMeshTestCase5_1::DoRun (void)
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
    
//     Vector3D pos = nodes.Get(0)->GetObject<MobilityModel>()->GetPosition();
//     pos.x = pos.x + 1;
//     nodes.Get(1)->GetObject<MobilityModel>()->SetPosition(pos);
    
    /*  make packet to send */
    Ptr<Packet> packet = Create<Packet>(50);
    LoRaMeshHeader header;
    header.SetType(DIRECTED);
    header.SetSrc(nodes.Get(0)->GetId());
    header.SetFwd(nodes.Get(0)->GetId());
    header.SetDest(nodes.Get(1)->GetId());
    packet->AddHeader(header);
    
    nodes.Get(0)->GetDevice(0)->Send(packet, Address(), 0);
    
    Simulator::Stop(Minutes(5));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/
/*  Test Case #5.2: Logging of Routing Updates  */
class LoRaMeshTestCase5_2 : public TestCase
{
public:
    LoRaMeshTestCase5_2 ();
    virtual ~LoRaMeshTestCase5_2 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase5_2::LoRaMeshTestCase5_2 ()
  : TestCase ("LoRa Mesh Test Case #5.2: Logging of Routing Updates")
{
}

LoRaMeshTestCase5_2::~LoRaMeshTestCase5_2 ()
{
}

void
LoRaMeshTestCase5_2::DoRun (void)
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
    
    Simulator::Stop(Minutes(2));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/
/*  Test Case #5.3: Logging of Packet Collisions    */
class LoRaMeshTestCase5_3 : public TestCase
{
public:
    LoRaMeshTestCase5_3 ();
    virtual ~LoRaMeshTestCase5_3 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase5_3::LoRaMeshTestCase5_3 ()
  : TestCase ("LoRa Mesh Test Case #5.3: Logging of Packet Collisions")
{
}

LoRaMeshTestCase5_3::~LoRaMeshTestCase5_3 ()
{
}

void
LoRaMeshTestCase5_3::DoRun (void)
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
        
        phy->SetNetDevice(device);
        phy->SetMAC(mac);
        phy->SetChannel(channel);
        mac->SetMinDelay(0);
        mac->SetMaxDelay(1);
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
    
    Simulator::Stop(Seconds(5));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/
/*  Test Case #5.4: Ascii Tracing    */
class LoRaMeshTestCase5_4 : public TestCase
{
public:
    LoRaMeshTestCase5_4 ();
    virtual ~LoRaMeshTestCase5_4 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase5_4::LoRaMeshTestCase5_4 ()
  : TestCase ("LoRa Mesh Test Case #5.4: Ascii Tracing")
{
}

LoRaMeshTestCase5_4::~LoRaMeshTestCase5_4 ()
{
}

void
LoRaMeshTestCase5_4::DoRun (void)
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
    
    AsciiHelperForLoRa ascii;
    ascii.EnableAscii("", nodes);
    
    /*  make packet to send */
    Ptr<Packet> packet = Create<Packet>(50);
    LoRaMeshHeader header;
    header.SetType(DIRECTED);
    header.SetSrc(nodes.Get(0)->GetId());
    header.SetFwd(nodes.Get(0)->GetId());
    header.SetDest(nodes.Get(1)->GetId());
    packet->AddHeader(header);
    
    nodes.Get(0)->GetDevice(0)->Send(packet, Address(), 0);
    
    Simulator::Stop(Minutes(3));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/
/*  Test Case #5.5: PDR Calculation */
class LoRaMeshTestCase5_5 : public TestCase
{
public:
    LoRaMeshTestCase5_5 ();
    virtual ~LoRaMeshTestCase5_5 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase5_5::LoRaMeshTestCase5_5 ()
  : TestCase ("LoRa Mesh Test Case #5.5: PDR Calculation")
{
}

LoRaMeshTestCase5_5::~LoRaMeshTestCase5_5 ()
{
}

void
LoRaMeshTestCase5_5::DoRun (void)
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
    
    Ptr<RandomDiscPositionAllocator> position = CreateObject<RandomDiscPositionAllocator>();
    
    mobility.SetPositionAllocator(position);
    mobility.SetMobilityModel(  "ns3::RandomWaypointMobilityModel",
                                "Speed", StringValue ("ns3::UniformRandomVariable[Min=0|Max=60]"),
                                "Pause", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"),
                                "PositionAllocator", PointerValue(position)
    );
    
    nodes.Create(2);
    mobility.Install(nodes);
    
    for (NodeContainer::Iterator i = nodes.Begin();i != nodes.End();++i)
    {
        Ptr<Node> node = *i;
        device = Create<LoRaNetDevice>();
        phy = CreateObject<LoRaPHY>();
        mac = CreateObject<LoRaMAC>();
        
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
    
    AsciiHelperForLoRa ascii;
    ascii.EnableAscii("", nodes);
    
    for (unsigned int i = 0;i < 10;i++)
    {
        /*  make packet to send */
        Ptr<Packet> packet = Create<Packet>(50);
        LoRaMeshHeader header;
        header.SetType(DIRECTED);
        header.SetSrc(nodes.Get(0)->GetId());
        header.SetFwd(nodes.Get(0)->GetId());
        header.SetDest(nodes.Get(1)->GetId());
        packet->AddHeader(header);
        
        nodes.Get(0)->GetDevice(0)->Send(packet, Address(), 0);
    }
    
    Simulator::Stop(Minutes(10));
    Simulator::Run();
    Simulator::Destroy();
    
    return;
}

/************************************************************************************/


class LoRaMeshTestSuite_5 : public TestSuite
{
public:
    LoRaMeshTestSuite_5 ();
};

LoRaMeshTestSuite_5::LoRaMeshTestSuite_5 ()
    : TestSuite ("lora-mesh-5", UNIT)
{
    Time::SetResolution (Time::NS);
    LogComponentEnableAll(LOG_PREFIX_TIME);
    LogComponentEnable("LoRaMeshTestSuite_5", LOG_LEVEL_ALL);
    LogComponentEnable("LoRaPHY", LOG_LEVEL_ALL);
    LogComponentEnable("LoRaMAC", LOG_LEVEL_ALL);
    LogComponentEnable("LoRaChannel", LOG_LEVEL_ALL);
    
    // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
    AddTestCase (new LoRaMeshTestCase5_1, TestCase::TAKES_FOREVER);
    AddTestCase (new LoRaMeshTestCase5_2, TestCase::TAKES_FOREVER);
    AddTestCase (new LoRaMeshTestCase5_3, TestCase::TAKES_FOREVER);
    AddTestCase (new LoRaMeshTestCase5_4, TestCase::TAKES_FOREVER);
    AddTestCase (new LoRaMeshTestCase5_5, TestCase::TAKES_FOREVER);
}

// Do not forget to allocate an instance of this TestSuite
static LoRaMeshTestSuite_5 loraMeshTestSuite_5;
