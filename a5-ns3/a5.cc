#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/netanim-module.h"

/* Network Topology
* 
*                            Wifi 20.1.3.0
*                                 AP
*          *     *     *     *     *     *     *     *     *    * 
*          |     |     |     |     |     |     |     |     |    |    20.1.1.0
*         n17   n16   n15   n14   n13   n12   n11   n10   n9   n0 -------------- n1   n2   n3   n4   n5   n6   n7   n8
*                                                                 point-to-point  |    |    |    |    |    |    |    |
*                                                                                 =====================================
*                                                                                          LAN 20.1.3.0
*/

using namespace ns3;

#define NETANIM
#define VERBOSE

NS_LOG_COMPONENT_DEFINE("assignment5");

int main(int argc, char *argv[])
{
    uint32_t nCsma = 7;
    uint32_t nWifi = 9;
    uint32_t nP2p = 2;
    bool tracing = false;

    CommandLine cmd(__FILE__);
    cmd.AddValue("nCsma", "Number of \"extra\" CSMA devices", nCsma);
    cmd.AddValue("nWifi", "Number of wifi STA devices", nWifi);
    cmd.AddValue("tracing", "Enable pcap tracing", tracing);
    cmd.Parse(argc, argv);

#ifdef VERBOSE
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
#endif

    NodeContainer p2pNodes;
    p2pNodes.Create(nP2p);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("5ms"));

    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install(p2pNodes);

    NodeContainer csmaNodes;
    csmaNodes.Add(p2pNodes.Get(1));
    csmaNodes.Create(nCsma);

    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

    NetDeviceContainer csmaDevices;
    csmaDevices = csma.Install(csmaNodes);

    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(nWifi);
    NodeContainer wifiApNode = p2pNodes.Get(0);

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy = YansWifiPhyHelper::Default();
    phy.SetChannel(channel.Create());

    WifiHelper wifi;
    WifiMacHelper mac;
    Ssid ssid = Ssid("ns-3-ssid");
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid));

    NetDeviceContainer staDevices;
    staDevices = wifi.Install(phy, mac, wifiStaNodes);
    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));

    NetDeviceContainer apDevices;
    apDevices = wifi.Install(phy, mac, wifiApNode);

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(p2pNodes);
    mobility.Install(csmaNodes);

    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                    "MinX", DoubleValue(20.0), 
                                    "MinY", DoubleValue(20.0), 
                                    "DeltaX", DoubleValue(20.0), 
                                    "DeltaY", DoubleValue(20.0), 
                                    "GridWidth", UintegerValue(5), "
                                    LayoutType", StringValue("RowFirst"));
                                    
    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds", RectangleValue(Rectangle(-500, 500, -500, 500)),
                              "Speed", StringValue("ns3::ConstantRandomVariable[Constant=2]"),
                              "Pause", StringValue("ns3::ConstantRandomVariable[Constant=0.2]"));
    mobility.Install(wifiStaNodes);
    mobility.Install(wifiApNode);

    InternetStackHelper stack;
    stack.Install(csmaNodes);
    stack.Install(wifiApNode);
    stack.Install(wifiStaNodes);

    Ipv4AddressHelper address;

    address.SetBase("20.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(p2pDevices);

    address.SetBase("20.1.2.0", "255.255.255.0");
    address.Assign(staDevices);
    address.Assign(apDevices);

    address.SetBase("20.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces;
    csmaInterfaces = address.Assign(csmaDevices);

    UdpEchoServerHelper echoServer(9);

    ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(nCsma));
    serverApps.Start(Seconds(0.5));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(csmaInterfaces.GetAddress(nCsma), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(100));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(wifiStaNodes.Get(nWifi - 1));
    clientApps.Start(Seconds(0.75));
    clientApps.Stop(Seconds(10.0));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    Simulator::Stop(Seconds(10.0));

#ifdef NETANIM

    AnimationInterface anim("assignment5-anim.xml");

    //Point to Point
    anim.SetConstantPosition(p2pNodes.Get(0), 100, 15, 0);
    anim.UpdateNodeDescription(p2pNodes.Get(0), "n0");
    anim.UpdateNodeColor(p2pNodes.Get(0), 0, 255, 0);
    anim.SetConstantPosition(p2pNodes.Get(1), 110, 15, 0);
    anim.UpdateNodeDescription(p2pNodes.Get(1), "n1");
    anim.UpdateNodeColor(p2pNodes.Get(1), 0, 255, 0);

    //CSMA
    anim.SetConstantPosition(csmaNodes.Get(1), 120, 15, 0);
    anim.UpdateNodeDescription(csmaNodes.Get(1), "n2");
    anim.UpdateNodeColor(csmaNodes.Get(1), 0, 0, 255);
    anim.SetConstantPosition(csmaNodes.Get(2), 130, 15, 0);
    anim.UpdateNodeDescription(csmaNodes.Get(2), "n3");
    anim.UpdateNodeColor(csmaNodes.Get(2), 0, 0, 255);
    anim.SetConstantPosition(csmaNodes.Get(3), 140, 15, 0);
    anim.UpdateNodeDescription(csmaNodes.Get(3), "n4");
    anim.UpdateNodeColor(csmaNodes.Get(3), 0, 0, 255);
    anim.SetConstantPosition(csmaNodes.Get(4), 150, 15, 0);
    anim.UpdateNodeDescription(csmaNodes.Get(4), "n5");
    anim.UpdateNodeColor(csmaNodes.Get(4), 0, 0, 255);
    anim.SetConstantPosition(csmaNodes.Get(5), 160, 15, 0);
    anim.UpdateNodeDescription(csmaNodes.Get(5), "n6");
    anim.UpdateNodeColor(csmaNodes.Get(5), 0, 0, 255);
    anim.SetConstantPosition(csmaNodes.Get(6), 170, 15, 0);
    anim.UpdateNodeDescription(csmaNodes.Get(6), "n7");
    anim.UpdateNodeColor(csmaNodes.Get(6), 0, 0, 255);
    anim.SetConstantPosition(csmaNodes.Get(7), 180, 15, 0);
    anim.UpdateNodeDescription(csmaNodes.Get(7), "n8");
    anim.UpdateNodeColor(csmaNodes.Get(7), 0, 0, 255);

    //Wifi
    anim.SetConstantPosition(wifiStaNodes.Get(0), 0, 15, 0);
    anim.UpdateNodeDescription(wifiStaNodes.Get(0), "n9");
    anim.UpdateNodeColor(wifiStaNodes.Get(0), 255, 0, 0);
    anim.SetConstantPosition(wifiStaNodes.Get(1), 10, 15, 0);
    anim.UpdateNodeDescription(wifiStaNodes.Get(1), "n10");
    anim.UpdateNodeColor(wifiStaNodes.Get(1), 255, 0, 0);
    anim.SetConstantPosition(wifiStaNodes.Get(2), 20, 15, 0);
    anim.UpdateNodeDescription(wifiStaNodes.Get(2), "n11");
    anim.UpdateNodeColor(wifiStaNodes.Get(2), 255, 0, 0);
    anim.SetConstantPosition(wifiStaNodes.Get(3), 30, 15, 0);
    anim.UpdateNodeDescription(wifiStaNodes.Get(3), "n12");
    anim.UpdateNodeColor(wifiStaNodes.Get(3), 255, 0, 0);
    anim.SetConstantPosition(wifiStaNodes.Get(4), 40, 15, 0);
    anim.UpdateNodeDescription(wifiStaNodes.Get(4), "n13");
    anim.UpdateNodeColor(wifiStaNodes.Get(4), 255, 0, 0);
    anim.SetConstantPosition(wifiStaNodes.Get(5), 50, 15, 0);
    anim.UpdateNodeDescription(wifiStaNodes.Get(5), "n14");
    anim.UpdateNodeColor(wifiStaNodes.Get(5), 255, 0, 0);
    anim.SetConstantPosition(wifiStaNodes.Get(6), 60, 15, 0);
    anim.UpdateNodeDescription(wifiStaNodes.Get(6), "n15");
    anim.UpdateNodeColor(wifiStaNodes.Get(6), 255, 0, 0);
    anim.SetConstantPosition(wifiStaNodes.Get(7), 70, 15, 0);
    anim.UpdateNodeDescription(wifiStaNodes.Get(7), "n16");
    anim.UpdateNodeColor(wifiStaNodes.Get(7), 255, 0, 0);
    anim.SetConstantPosition(wifiStaNodes.Get(8), 80, 15, 0);
    anim.UpdateNodeDescription(wifiStaNodes.Get(8), "n17");
    anim.UpdateNodeColor(wifiStaNodes.Get(8), 255, 0, 0);
#endif

    if(tracing == true)
    {
        pointToPoint.EnablePcapAll("assignment5");
        phy.EnablePcap("assignment5", apDevices.Get(0));
        csma.EnablePcap("assignment5", csmaDevices.Get(0), true);
    }

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}

