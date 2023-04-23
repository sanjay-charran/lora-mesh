#include "ns3/core-module.h"
#include "ns3/lora-mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/application.h"
#include "ns3/mobility-helper.h"
#include "ns3/application-container.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/correlated-shadowing-propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/mobility-helper.h"
#include "ns3/node-container.h"
#include "ns3/ptr.h"
#include "ns3/net-device-container.h"
#include "ns3/object-factory.h"
#include "ns3/vector.h"
#include "ns3/buildings-module.h"
#include "ns3/building-container.h"
#include "ns3/buildings-helper.h"

#include "ns3/lora-phy.h"
#include "ns3/lora-mac.h"
#include "ns3/lora-net-device.h"
#include "ns3/lora-channel.h"

#include <iterator>
#include <vector>

#define NUM_NODES       21
#define SIMULATION_SF   12

using namespace ns3;
using namespace lora_mesh;

double sx1278_RxSens[7] = {-118, -123, -126, -129, -132, -133, -136};   //for BW=125kHz

NS_LOG_COMPONENT_DEFINE ("UrbanScenarioExample");

int
main (int argc, char *argv[])
{
    Time::SetResolution (Time::NS);
    LogComponentEnableAll(LOG_PREFIX_TIME);
    LogComponentEnable("LoRaMAC", LOG_LEVEL_ALL);
    LogComponentEnable("LoRaPHY", LOG_LEVEL_ALL);
    NS_LOG_UNCOND ("LoRa Mesh Urban Scenario Example..." << std::endl);
    
    //create channel
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent (4);
    loss->SetReference (1, 7.7);
    Ptr<BuildingPenetrationLoss> buildingLoss = CreateObject<BuildingPenetrationLoss>();
    
    loss->SetNext(buildingLoss);
    channel->SetLossModel(loss);
    channel->SetDelayModel(delay);
    
    //mobility
    MobilityHelper mobility;
    
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    
    //setup buildings
    BuildingContainer buildings;
    Ptr<Building> building;
    
    building = CreateObject<Building>();
    building->SetBoundaries(Box(242.718, 339.806, 312.46, 394.888, 0, 20));
    building->SetBuildingType(Building::Commercial);
    building->SetExtWallsType(Building::ConcreteWithWindows);
    building->SetNFloors(1);
    building->SetNRoomsX(1);
    building->SetNRoomsY(1);
    buildings.Add(building);
    
    building = CreateObject<Building>();
    building->SetBoundaries(Box(217.476, 423.301, 490.735, 600, 0, 20));
    building->SetBuildingType(Building::Commercial);
    building->SetExtWallsType(Building::ConcreteWithWindows);
    building->SetNFloors(1);
    building->SetNRoomsX(1);
    building->SetNRoomsY(1);
    buildings.Add(building);
    
    building = CreateObject<Building>();
    building->SetBoundaries(Box(326.214, 423.301, 95.847, 228.115, 0, 20));
    building->SetBuildingType(Building::Commercial);
    building->SetExtWallsType(Building::ConcreteWithWindows);
    building->SetNFloors(1);
    building->SetNRoomsX(1);
    building->SetNRoomsY(1);
    buildings.Add(building);
    
    building = CreateObject<Building>();
    building->SetBoundaries(Box(266.019, 423.301, 0, 84.345, 0, 20));
    building->SetBuildingType(Building::Commercial);
    building->SetExtWallsType(Building::ConcreteWithWindows);
    building->SetNFloors(1);
    building->SetNRoomsX(1);
    building->SetNRoomsY(1);
    buildings.Add(building);
    
    building = CreateObject<Building>();
    building->SetBoundaries(Box(436.893, 557.282, 394.888, 467.732, 0, 20));
    building->SetBuildingType(Building::Commercial);
    building->SetExtWallsType(Building::ConcreteWithWindows);
    building->SetNFloors(1);
    building->SetNRoomsX(1);
    building->SetNRoomsY(1);
    buildings.Add(building);
    
    building = CreateObject<Building>();
    building->SetBoundaries(Box(568.932, 702.913, 431.31, 467.732, 0, 20));
    building->SetBuildingType(Building::Commercial);
    building->SetExtWallsType(Building::ConcreteWithWindows);
    building->SetNFloors(1);
    building->SetNRoomsX(1);
    building->SetNRoomsY(1);
    buildings.Add(building);
    
    building = CreateObject<Building>();
    building->SetBoundaries(Box(582.524, 691.262, 228.115, 287.54, 0, 20));
    building->SetBuildingType(Building::Commercial);
    building->SetExtWallsType(Building::ConcreteWithWindows);
    building->SetNFloors(1);
    building->SetNRoomsX(1);
    building->SetNRoomsY(1);
    buildings.Add(building);
    
    building = CreateObject<Building>();
    building->SetBoundaries(Box(714.563, 800, 431.31, 563.578, 0, 20));
    building->SetBuildingType(Building::Commercial);
    building->SetExtWallsType(Building::ConcreteWithWindows);
    building->SetNFloors(1);
    building->SetNRoomsX(1);
    building->SetNRoomsY(1);
    buildings.Add(building);
    
    //setup nodes
    NodeContainer loranodes;
    loranodes.Create(NUM_NODES);
    
    mobility.Install(loranodes);
    BuildingsHelper::Install(loranodes);
    
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
        //sx1278
        phy->SetRxSens(sx1278_RxSens[SIMULATION_SF - 6]); //dBm
        phy->SetTxPower(20);    //dBm
        phy->SetRxFreq(860);    //MHz
        phy->SetTxFreq(860);    //MHz
        phy->SetTxSF(SIMULATION_SF);    
        phy->SetRxSF(SIMULATION_SF);
        phy->SetTxBW(125000);   //Hz
        
        channel->AddPHY(phy);
        device->SetMAC(mac);
        device->SetPHY(phy);
        device->SetNode(node);
        node->AddDevice(device);        
        mac->SetMinDelay(0);
        mac->SetMaxDelay(20);
        mac->SetRoutingUpdateFrequency(2);
        mac->SetPHY(phy);
        mac->SetDevice(device);
    }
    
    ApplicationContainer apps;
    
    for (NodeContainer::Iterator i = loranodes.Begin();i != loranodes.End(); ++i)
    {
        Ptr<Application> app = CreateObject<Application>();
        
        app->SetNode(*i);
        (*i)->AddApplication(app);
        
        apps.Add(app);
    }
    
    loranodes.Get(6)->GetDevice(0)->GetObject<LoRaNetDevice>()->GetPHY()->SwitchStateSLEEP();    //node 6 was missing in scaenario
    
    //set node locations
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
    
    AsciiHelperForLoRa ascii;
    ascii.EnableAscii("UrbanScenario", loranodes);
    
    for (NodeContainer::Iterator i = loranodes.Begin() + 1;i != loranodes.End();++i)
    {
        Ptr<Node> node = *i;
        
        for (unsigned int j = 0;j < 10;j++)
        {
            Ptr<Packet> packet = Create<Packet>(50);
            LoRaMeshHeader header;
                
            header.SetType(DIRECTED);
            header.SetSrc(node->GetId());
            header.SetFwd(node->GetId());
            header.SetDest(loranodes.Get(0)->GetId());
            packet->AddHeader(header);
            
            node->GetDevice(0)->Send(packet, Address(), 0);
        }
    }
    
    //simulator setup
    Simulator::Stop(Hours(100));
    Simulator::Run ();
    Simulator::Destroy ();
    
    return 0;
}
