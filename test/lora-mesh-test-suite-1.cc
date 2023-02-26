/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/lora-mesh.h"

#include "ns3/ptr.h"
#include "ns3/core-module.h"
#include "ns3/test.h"
#include "ns3/node-container.h"
#include "ns3/vector.h"
#include "ns3/mobility-helper.h"

#include <iterator>

using namespace ns3;
using namespace lora_mesh;

NS_LOG_COMPONENT_DEFINE ("LoRaMeshTestSuite_1");

/************************************************************************************/
/*  Test Case #1.1: Node Creation   */
class LoRaMeshTestCase1_1 : public TestCase
{
public:
    LoRaMeshTestCase1_1 ();
    virtual ~LoRaMeshTestCase1_1 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_1::LoRaMeshTestCase1_1 ()
  : TestCase ("LoRa Mesh Test Case #1.1: Node Creation")
{
}

LoRaMeshTestCase1_1::~LoRaMeshTestCase1_1 ()
{
}

void
LoRaMeshTestCase1_1::DoRun (void)
{
    Ptr<Node> node = CreateObject<Node>();
    
    NS_TEST_ASSERT_MSG_NE(node, 0, "Test Case #1.1: Failed to Create Node");
    
    return;
}

/************************************************************************************/
/*  Test Case #1.2: Unique Node IDs  */
class LoRaMeshTestCase1_2 : public TestCase
{
public:
    LoRaMeshTestCase1_2 ();
    virtual ~LoRaMeshTestCase1_2 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_2::LoRaMeshTestCase1_2()
  : TestCase ("LoRa Mesh Test Case #1.2: Unique Node IDs")
{
}

LoRaMeshTestCase1_2::~LoRaMeshTestCase1_2 ()
{
}

void
LoRaMeshTestCase1_2::DoRun (void)
{
    NodeContainer nodes;
    nodes.Create(2);
    NodeContainer::Iterator i = nodes.Begin();
    NodeContainer::Iterator j = nodes.Begin();
    ++j;
    
    NS_TEST_ASSERT_MSG_NE((*i)->GetId(), (*j)->GetId(), "Test Case #1.2: Node IDs NOT Unique");
    
    return;
}

/************************************************************************************/
/*  Test Case #1.3: Node Deletion  */
class LoRaMeshTestCase1_3 : public TestCase
{
public:
    LoRaMeshTestCase1_3 ();
    virtual ~LoRaMeshTestCase1_3 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_3::LoRaMeshTestCase1_3()
  : TestCase ("LoRa Mesh Test Case #1.3: Node Deletion")
{
}

LoRaMeshTestCase1_3::~LoRaMeshTestCase1_3 ()
{
}

void
LoRaMeshTestCase1_3::DoRun (void)
{
    Ptr<Node> node = CreateObject<Node>();
    node = Ptr<Node> ();
    
    NS_TEST_ASSERT_MSG_EQ(node, 0, "Test Case #1.3: Failed to Delete Node");
    
    return;
}


/************************************************************************************/
/*  Test Case #1.4: Setting Node Location   */
class LoRaMeshTestCase1_4 : public TestCase
{
public:
    LoRaMeshTestCase1_4 ();
    virtual ~LoRaMeshTestCase1_4 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_4::LoRaMeshTestCase1_4()
  : TestCase ("LoRa Mesh Test Case #1.4: Setting Node Location")
{
}

LoRaMeshTestCase1_4::~LoRaMeshTestCase1_4 ()
{
}

void
LoRaMeshTestCase1_4::DoRun (void)
{
    Ptr<Node> node = CreateObject<Node>();
    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(node);
    
    Vector3D old_pos, new_pos;
    old_pos = node->GetObject<MobilityModel>()->GetPosition();
    
    new_pos.x = old_pos.x + 1;
    new_pos.y = old_pos.y + 1;
    new_pos.z = old_pos.z + 1;
    
    node->GetObject<MobilityModel>()->SetPosition(new_pos);
    new_pos = node->GetObject<MobilityModel>()->GetPosition();
    
    
    NS_TEST_ASSERT_MSG_NE(new_pos.x, old_pos.x, "Test Case #1.4: Failed to Set Node Position");
    
    return;
}


/************************************************************************************/
/*  Test Case #1.5: Setting Node Tx Power   */
class LoRaMeshTestCase1_5 : public TestCase
{
public:
    LoRaMeshTestCase1_5 ();
    virtual ~LoRaMeshTestCase1_5 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_5::LoRaMeshTestCase1_5()
  : TestCase ("LoRa Mesh Test Case #1.5: Setting Node Tx Power")
{
}

LoRaMeshTestCase1_5::~LoRaMeshTestCase1_5 ()
{
}

void
LoRaMeshTestCase1_5::DoRun (void)
{
    double old_pow, new_pow;
    
    Ptr<Node> node = CreateObject<Node>();
    Ptr<LoRaPHY> phy = CreateObject<LoRaPHY>();
    Ptr<LoRaMAC> mac = CreateObject<LoRaMAC>();
    Ptr<LoRaNetDevice> device = CreateObject<LoRaNetDevice>();
    
    phy->SetNetDevice(device);
    phy->SetMAC(mac);
    device->SetMAC(mac);
    device->SetPHY(phy);
    device->SetNode(node);
    node->AddDevice(device);
    mac->SetPHY(phy);
    mac->SetDevice(device);
    
    old_pow = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetTxPower();
    node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->SetTxPower(old_pow + 10);
    new_pow = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetTxPower();
    
    NS_TEST_ASSERT_MSG_NE(new_pow, old_pow, "Test Case #1.5: Failed to Set Node Tx Power");
    
    return;
}
/************************************************************************************/
/*  Test Case #1.6: Setting Node Tx Freq   */
class LoRaMeshTestCase1_6 : public TestCase
{
public:
    LoRaMeshTestCase1_6 ();
    virtual ~LoRaMeshTestCase1_6 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_6::LoRaMeshTestCase1_6()
  : TestCase ("LoRa Mesh Test Case #1.6: Setting Node Tx Freq")
{
}

LoRaMeshTestCase1_6::~LoRaMeshTestCase1_6 ()
{
}

void
LoRaMeshTestCase1_6::DoRun (void)
{
    double old_freq, new_freq;
    
    Ptr<Node> node = CreateObject<Node>();
    Ptr<LoRaPHY> phy = CreateObject<LoRaPHY>();
    Ptr<LoRaMAC> mac = CreateObject<LoRaMAC>();
    Ptr<LoRaNetDevice> device = CreateObject<LoRaNetDevice>();
    
    phy->SetNetDevice(device);
    phy->SetMAC(mac);
    device->SetMAC(mac);
    device->SetPHY(phy);
    device->SetNode(node);
    node->AddDevice(device);
    mac->SetPHY(phy);
    mac->SetDevice(device);
    
    old_freq = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetTxFreq();
    node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->SetTxFreq(old_freq + 10);
    new_freq = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetTxFreq();
    
    NS_TEST_ASSERT_MSG_NE(new_freq, old_freq, "Test Case #1.6: Failed to Set Node Tx Freq");
    
    return;
}
/************************************************************************************/
/*  Test Case #1.7: Setting Node Rx Sensitivity */
class LoRaMeshTestCase1_7 : public TestCase
{
public:
    LoRaMeshTestCase1_7 ();
    virtual ~LoRaMeshTestCase1_7 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_7::LoRaMeshTestCase1_7()
  : TestCase ("LoRa Mesh Test Case #1.7: Setting Node Rx Sensitivity")
{
}

LoRaMeshTestCase1_7::~LoRaMeshTestCase1_7 ()
{
}

void
LoRaMeshTestCase1_7::DoRun (void)
{
    double old_pow, new_pow;
    
    Ptr<Node> node = CreateObject<Node>();
    Ptr<LoRaPHY> phy = CreateObject<LoRaPHY>();
    Ptr<LoRaMAC> mac = CreateObject<LoRaMAC>();
    Ptr<LoRaNetDevice> device = CreateObject<LoRaNetDevice>();
    
    phy->SetNetDevice(device);
    phy->SetMAC(mac);
    device->SetMAC(mac);
    device->SetPHY(phy);
    device->SetNode(node);
    node->AddDevice(device);
    mac->SetPHY(phy);
    mac->SetDevice(device);
    
    old_pow = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetRxSens();
    node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->SetRxSens(old_pow + 10);
    new_pow = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetRxSens();
    
    NS_TEST_ASSERT_MSG_NE(new_pow, old_pow, "Test Case #1.7: Failed to Set Node Rx Sensitivity");
    
    return;
}
/************************************************************************************/
/*  Test Case #1.8: Setting Node Rx Freq   */
class LoRaMeshTestCase1_8 : public TestCase
{
public:
    LoRaMeshTestCase1_8 ();
    virtual ~LoRaMeshTestCase1_8 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_8::LoRaMeshTestCase1_8()
  : TestCase ("LoRa Mesh Test Case #1.8: Setting Node Rx Freq")
{
}

LoRaMeshTestCase1_8::~LoRaMeshTestCase1_8 ()
{
}

void
LoRaMeshTestCase1_8::DoRun (void)
{
    double old_freq, new_freq;
    
    Ptr<Node> node = CreateObject<Node>();
    Ptr<LoRaPHY> phy = CreateObject<LoRaPHY>();
    Ptr<LoRaMAC> mac = CreateObject<LoRaMAC>();
    Ptr<LoRaNetDevice> device = CreateObject<LoRaNetDevice>();
    
    phy->SetNetDevice(device);
    phy->SetMAC(mac);
    device->SetMAC(mac);
    device->SetPHY(phy);
    device->SetNode(node);
    node->AddDevice(device);
    mac->SetPHY(phy);
    mac->SetDevice(device);
    
    old_freq = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetRxFreq();
    node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->SetRxFreq(old_freq + 10);
    new_freq = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetRxFreq();
    
    NS_TEST_ASSERT_MSG_NE(new_freq, old_freq, "Test Case #1.8: Failed to Set Node Rx Freq");
    
    return;
}
/************************************************************************************/
/*  Test Case #1.9: Setting Node Tx Spreading Factor   */
class LoRaMeshTestCase1_9 : public TestCase
{
public:
    LoRaMeshTestCase1_9 ();
    virtual ~LoRaMeshTestCase1_9 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_9::LoRaMeshTestCase1_9 ()
  : TestCase ("LoRa Mesh Test Case #1.9: Setting Node Tx Spreading Factor")
{
}

LoRaMeshTestCase1_9::~LoRaMeshTestCase1_9 ()
{
}

void
LoRaMeshTestCase1_9::DoRun (void)
{
    uint8_t old_sf, new_sf;
    
    Ptr<Node> node = CreateObject<Node>();
    Ptr<LoRaPHY> phy = CreateObject<LoRaPHY>();
    Ptr<LoRaMAC> mac = CreateObject<LoRaMAC>();
    Ptr<LoRaNetDevice> device = CreateObject<LoRaNetDevice>();
    
    phy->SetNetDevice(device);
    phy->SetMAC(mac);
    device->SetMAC(mac);
    device->SetPHY(phy);
    device->SetNode(node);
    node->AddDevice(device);
    mac->SetPHY(phy);
    mac->SetDevice(device);
    
    old_sf = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetTxSF();
    node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->SetTxSF(old_sf + 1);
    new_sf = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetTxSF();
    
    NS_TEST_ASSERT_MSG_NE(new_sf, old_sf, "Test Case #1.9: Failed to Set Node Tx Spreading Factor");
    
    return;
}
/************************************************************************************/
/*  Test Case #1.10: Setting Node Rx Spreading Factor   */
class LoRaMeshTestCase1_10 : public TestCase
{
public:
    LoRaMeshTestCase1_10 ();
    virtual ~LoRaMeshTestCase1_10 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_10::LoRaMeshTestCase1_10 ()
  : TestCase ("LoRa Mesh Test Case #1.10: Setting Node Rx Spreading Factor")
{
}

LoRaMeshTestCase1_10::~LoRaMeshTestCase1_10 ()
{
}

void
LoRaMeshTestCase1_10::DoRun (void)
{
    uint8_t old_sf, new_sf;
    
    Ptr<Node> node = CreateObject<Node>();
    Ptr<LoRaPHY> phy = CreateObject<LoRaPHY>();
    Ptr<LoRaMAC> mac = CreateObject<LoRaMAC>();
    Ptr<LoRaNetDevice> device = CreateObject<LoRaNetDevice>();
    
    phy->SetNetDevice(device);
    phy->SetMAC(mac);
    device->SetMAC(mac);
    device->SetPHY(phy);
    device->SetNode(node);
    node->AddDevice(device);
    mac->SetPHY(phy);
    mac->SetDevice(device);
    
    old_sf = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetRxSF();
    node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->SetRxSF(old_sf + 1);
    new_sf = node->GetObject<LoRaNetDevice>()->GetMAC()->GetPHY()->GetRxSF();
    
    NS_TEST_ASSERT_MSG_NE(new_sf, old_sf, "Test Case #1.10: Failed to Set Node Tx Spreading Factor");
    
    return;
}
/************************************************************************************/
/*  Test Case #1.11: Attaching Node to Channel   */
class LoRaMeshTestCase1_11 : public TestCase
{
public:
    LoRaMeshTestCase1_11 ();
    virtual ~LoRaMeshTestCase1_11 ();

private:
    virtual void DoRun (void);
};

LoRaMeshTestCase1_11::LoRaMeshTestCase1_11 ()
  : TestCase ("LoRa Mesh Test Case #1.11: Attaching Node to Channel")
{
}

LoRaMeshTestCase1_11::~LoRaMeshTestCase1_11 ()
{
}

void
LoRaMeshTestCase1_11::DoRun (void)
{
    std::size_t old_phys, new_phys;
    
    Ptr<Node> node = CreateObject<Node>();
    Ptr<LoRaPHY> phy = CreateObject<LoRaPHY>();
    Ptr<LoRaMAC> mac = CreateObject<LoRaMAC>();
    Ptr<LoRaNetDevice> device = CreateObject<LoRaNetDevice>();
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    phy->SetNetDevice(device);
    phy->SetMAC(mac);
    device->SetMAC(mac);
    device->SetPHY(phy);
    device->SetNode(node);
    node->AddDevice(device);
    mac->SetPHY(phy);
    mac->SetDevice(device);
    
    old_phys = channel->GetNDevices();
    channel->AddPHY(phy);
    new_phys = channel->GetNDevices();
    
    NS_TEST_ASSERT_MSG_GT(new_phys, old_phys, "Test Case #1.11: Failed to Attach Node to Channel");
    
    return;
}
/************************************************************************************/


class LoRaMeshTestSuite_1 : public TestSuite
{
public:
    LoRaMeshTestSuite_1 ();
};

LoRaMeshTestSuite_1::LoRaMeshTestSuite_1 ()
    : TestSuite ("lora-mesh-1", UNIT)
{
    LogComponentEnable("LoRaMeshTestSuite_1", LOG_LEVEL_ALL);
    
    // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
    AddTestCase (new LoRaMeshTestCase1_1, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase1_2, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase1_3, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase1_4, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase1_5, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase1_6, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase1_7, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase1_8, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase1_9, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase1_10, TestCase::QUICK);
    AddTestCase (new LoRaMeshTestCase1_11, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LoRaMeshTestSuite_1 loraMeshTestSuite_1;
