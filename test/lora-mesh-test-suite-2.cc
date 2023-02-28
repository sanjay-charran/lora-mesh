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
#include "ns3/propagation-loss-model.h"

#include <iterator>

using namespace ns3;
using namespace lora_mesh;

NS_LOG_COMPONENT_DEFINE ("LoRaMeshTestSuite_2");

/************************************************************************************/
/*  Test Case #2.1: Channel Creation   */
class LoRaMeshTestCase2_1 : public TestCase
{
public:
    LoRaMeshTestCase2_1 ();
    virtual ~LoRaMeshTestCase2_1 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase2_1::LoRaMeshTestCase2_1 ()
  : TestCase ("LoRa Mesh Test Case #2.1: Channel Creation")
{
}

LoRaMeshTestCase2_1::~LoRaMeshTestCase2_1 ()
{
}

void
LoRaMeshTestCase2_1::DoRun (void)
{
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    NS_TEST_ASSERT_MSG_NE(channel, 0, "Test Case #2.1: Failed to Create Channel");
    
    return;
}

/************************************************************************************/
/*  Test Case #2.2: Unique Channel IDs   */
class LoRaMeshTestCase2_2 : public TestCase
{
public:
    LoRaMeshTestCase2_2 ();
    virtual ~LoRaMeshTestCase2_2 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase2_2::LoRaMeshTestCase2_2 ()
  : TestCase ("LoRa Mesh Test Case #2.2: Unique Channel IDs")
{
}

LoRaMeshTestCase2_2::~LoRaMeshTestCase2_2 ()
{
}

void
LoRaMeshTestCase2_2::DoRun (void)
{
    Ptr<LoRaChannel> channel1 = CreateObject<LoRaChannel>();
    Ptr<LoRaChannel> channel2 = CreateObject<LoRaChannel>();
    
    NS_TEST_ASSERT_MSG_NE(channel1->GetId(), channel2->GetId(), "Test Case #2.2: Channel IDs NOT Unique");
    
    return;
}

/************************************************************************************/
/*  Test Case #2.3: Setting Channel Loss Model  */
class LoRaMeshTestCase2_3 : public TestCase
{
public:
    LoRaMeshTestCase2_3 ();
    virtual ~LoRaMeshTestCase2_3 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase2_3::LoRaMeshTestCase2_3 ()
  : TestCase ("LoRa Mesh Test Case #2.3: Setting Channel Loss Model")
{
}

LoRaMeshTestCase2_3::~LoRaMeshTestCase2_3 ()
{
}

void
LoRaMeshTestCase2_3::DoRun (void)
{
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    
    channel->SetLossModel(loss);
    
    NS_TEST_ASSERT_MSG_NE(channel->GetLossModel(), 0, "Test Case #2.3: Failed to Set Channel Loss Model");
    
    return;
}

/************************************************************************************/
/*  Test Case #2.5: Setting Channel Delay Model  */
class LoRaMeshTestCase2_5 : public TestCase
{
public:
    LoRaMeshTestCase2_5 ();
    virtual ~LoRaMeshTestCase2_5 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase2_5::LoRaMeshTestCase2_5 ()
  : TestCase ("LoRa Mesh Test Case #2.5: Setting Channel Delay Model")
{
}

LoRaMeshTestCase2_5::~LoRaMeshTestCase2_5 ()
{
}

void
LoRaMeshTestCase2_5::DoRun (void)
{
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    
    channel->SetDelayModel(delay);
    
    NS_TEST_ASSERT_MSG_NE(channel->GetDelayModel(), 0, "Test Case #2.3: Failed to Set Channel Delay Model");
    
    return;
}

/************************************************************************************/


class LoRaMeshTestSuite_2 : public TestSuite
{
public:
    LoRaMeshTestSuite_2 ();
};

LoRaMeshTestSuite_2::LoRaMeshTestSuite_2 ()
    : TestSuite ("lora-mesh-2", UNIT)
{
    LogComponentEnable("LoRaMeshTestSuite_2", LOG_LEVEL_ALL);
    
    // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
    AddTestCase (new LoRaMeshTestCase2_1, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase2_2, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase2_3, TestCase::QUICK);
    //skipped obstruction test
    AddTestCase (new LoRaMeshTestCase2_5, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LoRaMeshTestSuite_2 loraMeshTestSuite_2;
