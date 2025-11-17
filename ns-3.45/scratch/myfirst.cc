#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/string.h"
#include <iostream>
#include <string>
#include <vector>


using namespace ns3;

int main(int argc, char *argv[])
{
    Time::SetResolution(Time::NS);
    
    std::cout << "Hello ns-3 on arm64 Mac!" << std::endl;
    
    // 2つのノードを作成
    NodeContainer nodes;
    nodes.Create(2);
    
    // Point-to-Point接続を設定
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    
    NetDeviceContainer devices = pointToPoint.Install(nodes);
    
    // インターネットスタックをインストール
    InternetStackHelper stack;
    stack.Install(nodes);
    
    // IPアドレスを割り当て
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);
    
    // UDPエコーサーバーを設定
    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));
    
    // UDPエコークライアントを設定
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(3));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));
    
    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));
    
    // シミュレーション実行
    Simulator::Run();
    Simulator::Destroy();
    
    std::cout << "Simulation finished!" << std::endl;
    
    return 0;
}