/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/lora-mesh.h"

#include "ns3/ptr.h"
#include "ns3/core-module.h"
#include "ns3/test.h"

using namespace ns3;

/************************************************************************************/
class LoraMeshTestCase1_1 : public TestCase
{
public:
    LoraMeshTestCase1_1 ();
    virtual ~LoraMeshTestCase1_1 ();

private:
    virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
LoraMeshTestCase1_1::LoraMeshTestCase1_1 ()
  : TestCase ("LoRa Mesh Test Case #1.1: Node Creation")
{
}

LoraMeshTestCase1_1::~LoraMeshTestCase1_1 ()
{
}

void
LoraMeshTestCase1_1::DoRun (void)
{
    Ptr<Node> node = CreateObject<Node>();
    
    NS_TEST_ASSERT_MSG_NE(node, 0, "Test Case #1.1: Failed to Create Node");
    
    return;
}
/************************************************************************************/

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class LoraMeshTestSuite_1 : public TestSuite
{
public:
    LoraMeshTestSuite_1 ();
};

LoraMeshTestSuite_1::LoraMeshTestSuite_1 ()
    : TestSuite ("lora-mesh-1", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new LoraMeshTestCase1_1, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LoraMeshTestSuite_1 loraMeshTestSuite_1;
