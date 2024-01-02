#include "ns3/core-module.h"
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
#include "ns3/buildings-module.h"
#include "ns3/building-container.h"
#include "ns3/buildings-helper.h"

#include "ns3/lwb.h"
#include "ns3/lora-phy.h"
#include "ns3/lora-net-device.h"
#include "ns3/lora-channel.h"
#include "ns3/building-penetration-loss.h"

#include <iterator>
#include <vector>

#define NUM_NODES       5
#define SIMULATION_SF   7

using namespace ns3;
using namespace lora_mesh;

NS_LOG_COMPONENT_DEFINE ("LWBExample");

int
main (int argc, char *argv[])
{
    Time::SetResolution (Time::NS);
    LogComponentEnableAll(LOG_PREFIX_TIME);
    NS_LOG_UNCOND ("LWB Example..." << std::endl);
    
    Ptr<LoRaChannel> channel = CreateObject<LoRaChannel>();
    
    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent (4);
    loss->SetReference (1, 7.7);
    
	channel->SetLossModel(loss);
    channel->SetDelayModel(delay);

    MobilityHelper mobility;
    
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.SetPositionAllocator(	"ns3::RandomDiscPositionAllocator",
									"Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=1000]")
	);

    NodeContainer loranodes;
    loranodes.Create(NUM_NODES);
    
    mobility.Install(loranodes);
    
    Ptr<LoRaNetDevice> device;
    Ptr<LoRaPHY> phy;
    Ptr<LWB> mac;
    Ptr<Glossy> glossy;
    
	const double sx1278_RxSens[7] = {-118, -123, -126, -129, -132, -133, -136};   //for BW=125kHz
	
    for (NodeContainer::Iterator i = loranodes.Begin(); i != loranodes.End(); ++i)
    {
        Ptr<Node> node = *i;
        device = Create<LoRaNetDevice>();
        phy = CreateObject<LoRaPHY>();
        mac = CreateObject<LWB>();
        glossy = CreateObject<Glossy>();
        
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
        
        mac->SetNode(node);
        mac->SetPHY(phy);
        mac->SetDevice(device);
        mac->SetGlossy(glossy);
        glossy->SetNode(node);
        glossy->SetLWB(mac);
        glossy->SetNTxMax(1);
    }
    
    ApplicationContainer apps;
    
    for (NodeContainer::Iterator i = loranodes.Begin();i != loranodes.End(); ++i)
    {
        Ptr<Application> app = CreateObject<Application>();
        
        app->SetNode(*i);
        (*i)->AddApplication(app);
        
        apps.Add(app);
    }
    
    for (NodeContainer::Iterator i = loranodes.Begin();i != loranodes.End(); ++i)
    {
        Ptr<Node> node = *i;
        Ptr<Packet> packet = Create<Packet>(50);
        LWBHeader header;
        LWBDataPacketHeader dheader;
    
        dheader.SetRecipientId(1);
        header.SetPacketType(DATA);
        packet->AddHeader(dheader);
        packet->AddHeader(header);
        
        node->GetDevice(0)->Send(packet, Address(), 0);
    }
    
    loranodes.Get(0)->GetDevice(0)->GetObject<LoRaNetDevice>()->GetMAC()->GetObject<LWB>()->Start();
    
    //simulator setup
    Simulator::Stop(Minutes(10));
    Simulator::Run();
    Simulator::Destroy ();
    
    return 0;
}
