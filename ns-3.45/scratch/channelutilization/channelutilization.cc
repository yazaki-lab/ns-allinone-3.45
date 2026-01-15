/*
 * ns-3 無線LAN チャネル使用率シミュレーション (修正版v5)
 * 対応: 再送回数, 総送信回数, 再送率, 送信電力の追加
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"

#include <fstream>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <yaml-cpp/yaml.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("WifiChannelUtilizationSim");

// --- グローバル集計用変数 ---
uint64_t g_totalBusyTime = 0;       
uint64_t g_phyTxFrames = 0;         
uint64_t g_macTxAttempts = 0;       // 総送信回数 (初回 + 再送)
uint64_t g_macTxFailed = 0;         // 再送回数 (ACK未受信による再試行回数)

double g_totalRssi = 0.0;           
double g_totalSnr = 0.0;            
uint64_t g_rxSignalCount = 0;       

// --- シミュレーション設定構造体 ---
struct SimulationConfig {
    uint32_t nStations;
    uint32_t heavyUserPercentage;
    uint32_t nHeavyUsers;
    uint32_t nLightUsers;

    double radius;          
    std::string outputFile;
    uint32_t heavyUserRate; 
    uint32_t lightUserRate; 
    uint32_t packetSize;    
    double txPower;         // AP送信電力 (dBm) ※追加
    double simulationTime;  

    bool useTcp;            
    bool useMinstrel;       
    uint32_t maxAmpduSize;  
    uint32_t rtsCtsThreshold; 

    bool enableTxtOutput;
    bool enableNetAnim;
    bool verbose;
};

// --- コールバック関数群 ---

void PhyStateChangeCallback(std::string context, Time start, Time duration, WifiPhyState state) {
    if (state == WifiPhyState::TX || state == WifiPhyState::RX || state == WifiPhyState::CCA_BUSY) {
        g_totalBusyTime += duration.GetNanoSeconds();
    }
}

void PhyTxBeginCallback(std::string context, Ptr<const Packet> p, double txPowerW) {
    g_phyTxFrames++;
}

// MAC層でのデータ送信失敗（再送のトリガー）をカウント
void MacTxDataFailedCallback(std::string context, Mac48Address addr) {
    g_macTxFailed++;
}

// MAC層での送信試行（総送信回数）をカウント
void MacTxDataCallback(std::string context, Ptr<const Packet> p) {
    g_macTxAttempts++;
}

void MonitorSnifferRxCallback(std::string context, Ptr<const Packet> packet, uint16_t channelFreqMhz, 
                              WifiTxVector txVector, MpduInfo mpduInfo, SignalNoiseDbm signalNoise, uint16_t staId) {
    g_totalRssi += signalNoise.signal; 
    g_totalSnr += (signalNoise.signal - signalNoise.noise); 
    g_rxSignalCount++;
}

double CalculateChannelUtilization(double simulationTimeSec) {
    if (simulationTimeSec <= 0) return 0.0;
    double totalTimeNano = simulationTimeSec * 1000000000.0;
    return (double)g_totalBusyTime / totalTimeNano * 100.0;
}

SimulationConfig LoadConfigFromYAML(const std::string& configFile) {
    SimulationConfig config;
    try {
        YAML::Node yamlConfig = YAML::LoadFile(configFile);
        config.nStations = yamlConfig["nStations"].as<uint32_t>();
        config.heavyUserPercentage = yamlConfig["heavyUserPercentage"].as<uint32_t>();
        config.nHeavyUsers = (config.nStations * config.heavyUserPercentage) / 100;
        config.nLightUsers = config.nStations - config.nHeavyUsers;
        config.radius = yamlConfig["radius"].as<double>();
        config.outputFile = yamlConfig["outputFile"].as<std::string>();
        config.heavyUserRate = yamlConfig["heavyUserRate"].as<uint32_t>();
        config.lightUserRate = yamlConfig["lightUserRate"].as<uint32_t>();
        config.packetSize = yamlConfig["packetSize"].as<uint32_t>();
        config.txPower = yamlConfig["txPower"].as<double>(16.0206); // デフォルト値 ※追加
        config.useTcp = yamlConfig["useTcp"].as<bool>(false);
        config.useMinstrel = yamlConfig["useMinstrel"].as<bool>(false);
        config.maxAmpduSize = yamlConfig["maxAmpduSize"].as<uint32_t>(65535); 
        config.rtsCtsThreshold = yamlConfig["rtsCtsThreshold"].as<uint32_t>(2347); 
        config.simulationTime = yamlConfig["simulationTime"].as<double>(10.0);
        config.enableTxtOutput = yamlConfig["enableTxtOutput"].as<bool>(true);
        config.enableNetAnim = yamlConfig["enableNetAnim"].as<bool>(false);
        config.verbose = yamlConfig["verbose"].as<bool>(false);
    } catch (const YAML::Exception& e) {
        std::cerr << "YAML Error: " << e.what() << std::endl;
        exit(1);
    }
    return config;
}

void GenerateDefaultConfig(const std::string& filename) {
    std::ofstream out(filename);
    out << "nStations: 10" << std::endl;
    out << "heavyUserPercentage: 100" << std::endl;
    out << "radius: 10.0" << std::endl;
    out << "outputFile: \"experiment_result.csv\"" << std::endl;
    out << "heavyUserRate: 10" << std::endl;
    out << "lightUserRate: 2" << std::endl;
    out << "packetSize: 1500" << std::endl;
    out << "txPower: 16.0206" << std::endl; // ※追加
    out << "simulationTime: 10.0" << std::endl;
    out << "useTcp: false" << std::endl;
    out << "useMinstrel: false" << std::endl;
    out << "maxAmpduSize: 65535" << std::endl;
    out << "rtsCtsThreshold: 65535" << std::endl;
    out << "enableTxtOutput: true" << std::endl;
    out << "enableNetAnim: false" << std::endl;
    out << "verbose: false" << std::endl;
    out.close();
}

int main(int argc, char *argv[]) {
    RngSeedManager::SetSeed(1);
    RngSeedManager::SetRun(1);

    std::string configFile = "config.yaml";
    bool generateConfig = false;

    CommandLine cmd;
    cmd.AddValue("config", "YAML config file", configFile);
    cmd.AddValue("generate-config", "Generate default config", generateConfig);
    cmd.Parse(argc, argv);

    if (generateConfig) {
        GenerateDefaultConfig(configFile);
        return 0;
    }

    SimulationConfig config = LoadConfigFromYAML(configFile);

    if (config.verbose) {
        LogComponentEnable("WifiChannelUtilizationSim", LOG_LEVEL_INFO);
    }

    NodeContainer wifiApNode;
    wifiApNode.Create(1);
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(config.nStations);

    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211ac);

    if (config.useMinstrel) {
        wifi.SetRemoteStationManager("ns3::MinstrelHtWifiManager");
    } else {
        wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                     "DataMode", StringValue("VhtMcs8"),
                                     "ControlMode", StringValue("VhtMcs0"));
    }

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    phy.SetChannel(channel.Create());
    
    // YAMLから読み込んだ送信電力を設定 ※追加
    phy.Set("TxPowerStart", DoubleValue(config.txPower));
    phy.Set("TxPowerEnd", DoubleValue(config.txPower));

    WifiMacHelper mac;
    Ssid ssid = Ssid("ns3-research");

    mac.SetType("ns3::ApWifiMac", 
                "Ssid", SsidValue(ssid),
                "BE_MaxAmpduSize", UintegerValue(config.maxAmpduSize)); 
    NetDeviceContainer apDevice = wifi.Install(phy, mac, wifiApNode);

    mac.SetType("ns3::StaWifiMac", 
                "Ssid", SsidValue(ssid), 
                "ActiveProbing", BooleanValue(false),
                "BE_MaxAmpduSize", UintegerValue(config.maxAmpduSize)); 
    NetDeviceContainer staDevices = wifi.Install(phy, mac, wifiStaNodes);

    for (uint32_t i = 0; i < apDevice.GetN(); ++i) {
        Ptr<WifiNetDevice> dev = DynamicCast<WifiNetDevice>(apDevice.Get(i));
        dev->GetRemoteStationManager()->SetAttribute("RtsCtsThreshold", UintegerValue(config.rtsCtsThreshold));
    }
    for (uint32_t i = 0; i < staDevices.GetN(); ++i) {
        Ptr<WifiNetDevice> dev = DynamicCast<WifiNetDevice>(staDevices.Get(i));
        dev->GetRemoteStationManager()->SetAttribute("RtsCtsThreshold", UintegerValue(config.rtsCtsThreshold));
    }

    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(0.0, 0.0, 0.0)); // AP
    for (uint32_t i = 0; i < config.nStations; ++i) {
        double angle = (2.0 * M_PI * i) / config.nStations;
        positionAlloc->Add(Vector(config.radius * cos(angle), config.radius * sin(angle), 0.0));
    }
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(wifiApNode);
    mobility.Install(wifiStaNodes);

    InternetStackHelper stack;
    stack.Install(wifiApNode);
    stack.Install(wifiStaNodes);
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer apInterface = address.Assign(apDevice);
    Ipv4InterfaceContainer staInterfaces = address.Assign(staDevices);

    ApplicationContainer serverApps, clientApps;
    uint16_t port = 9000;

    for (uint32_t i = 0; i < config.nStations; ++i) {
        uint32_t dataRateMbps = (i < config.nHeavyUsers) ? config.heavyUserRate : config.lightUserRate;
        Address serverAddress(InetSocketAddress(apInterface.GetAddress(0), port + i));

        if (config.useTcp) {
            // 受信側 (Server: AP)
            PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port + i));
            serverApps.Add(packetSinkHelper.Install(wifiApNode.Get(0)));

            // 送信側 (Client: STA)
            // OnOffHelperを使うと、TCPでも指定レート(DataRate)で送信しようと制御します
            OnOffHelper onoff("ns3::TcpSocketFactory", serverAddress);
            
            // ずっとON(送信状態)にする設定
            onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
            onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
            
            // レートとパケットサイズの設定
            onoff.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(dataRateMbps) + "Mbps")));
            onoff.SetAttribute("PacketSize", UintegerValue(config.packetSize));
            
            clientApps.Add(onoff.Install(wifiStaNodes.Get(i)));
        } else {
            UdpServerHelper server(port + i);
            serverApps.Add(server.Install(wifiApNode.Get(0)));
            UdpClientHelper client(apInterface.GetAddress(0), port + i);
            double interval = (config.packetSize * 8.0) / (dataRateMbps * 1e6);
            client.SetAttribute("Interval", TimeValue(Seconds(interval)));
            client.SetAttribute("PacketSize", UintegerValue(config.packetSize));
            client.SetAttribute("MaxPackets", UintegerValue(4294967295u));
            clientApps.Add(client.Install(wifiStaNodes.Get(i)));
        }
    }

    serverApps.Start(Seconds(0.0));
    clientApps.Start(Seconds(1.0));

    // --- トレース接続 ---
    Config::Connect("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/State/State",
                    MakeCallback(&PhyStateChangeCallback));
    Config::Connect("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/PhyTxBegin",
                    MakeCallback(&PhyTxBeginCallback));
    Config::Connect("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/MonitorSnifferRx", 
                    MakeCallback(&MonitorSnifferRxCallback));

    for(uint32_t i=0; i<config.nStations; i++){
        std::stringstream path;
        path << "/NodeList/" << (i+1) << "/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/MacTxDataFailed";
        Config::Connect(path.str(), MakeCallback(&MacTxDataFailedCallback));
        
        std::stringstream path2;
        path2 << "/NodeList/" << (i+1) << "/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx";
        Config::Connect(path2.str(), MakeCallback(&MacTxDataCallback));
    }

    // AP(Node 0)側の送信試行・失敗もカウント対象に含める場合（下り通信メインなら重要）
    Config::Connect("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/MacTxDataFailed", 
                    MakeCallback(&MacTxDataFailedCallback));
    Config::Connect("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx", 
                    MakeCallback(&MacTxDataCallback));

    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    NS_LOG_INFO("Simulating...");
    Simulator::Stop(Seconds(config.simulationTime + 0.1));
    Simulator::Run();

    // --- 結果集計 ---
    double channelUtil = CalculateChannelUtilization(config.simulationTime);
    double collisionRate = (g_macTxAttempts > 0) ? (double)g_macTxFailed / (g_macTxAttempts + (double)g_macTxFailed ) * 100.0 : 0.0;
    
    // 再送率の計算 ※追加
    double retransRate = (g_macTxAttempts > 0) ? (double)g_macTxFailed / g_macTxAttempts * 100.0 : 0.0;

    monitor->CheckForLostPackets();
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();
    double totalThroughput = 0.0;
    long totalRxPackets = 0;
    long totalTxPackets = 0;
    double totalDelaySec = 0.0;

    for (auto const &flow : stats) {
        totalThroughput += flow.second.rxBytes * 8.0 / 1e6 / config.simulationTime;
        totalRxPackets += flow.second.rxPackets;
        totalTxPackets += flow.second.txPackets;
        totalDelaySec += flow.second.delaySum.GetSeconds();
    }

    double avgDelayMs = (totalRxPackets > 0) ? (totalDelaySec / totalRxPackets) * 1000.0 : 0.0;
    double packetLossRate = (totalTxPackets > 0) ? (1.0 - (double)totalRxPackets / totalTxPackets) * 100.0 : 0.0;
    double aggregationRatio = (g_phyTxFrames > 0) ? (double)totalRxPackets / g_phyTxFrames : 0.0;

    double avgRssi = (g_rxSignalCount > 0) ? (g_totalRssi / g_rxSignalCount) : 0.0;
    double avgSnr = (g_rxSignalCount > 0) ? (g_totalSnr / g_rxSignalCount) : 0.0;

    // コンソール出力
    std::cout << "=== Result ===" << std::endl;
    std::cout << "Stations:            " << config.nStations << std::endl;
    std::cout << "Channel Utilization: " << channelUtil << " %" << std::endl;
    std::cout << "Total Throughput:    " << totalThroughput << " Mbps" << std::endl;
    std::cout << "Total Mac Tx:        " << g_macTxAttempts << " times" << std::endl; // 追加
    std::cout << "Retransmissions:     " << g_macTxFailed << " times" << std::endl;   // 追加
    std::cout << "Retransmission Rate: " << retransRate << " %" << std::endl;       // 追加
    std::cout << "AP Tx Power:         " << config.txPower << " dBm" << std::endl;    // 追加
    std::cout << "Packet Loss Rate:    " << packetLossRate << " %" << std::endl;
    std::cout << "Avg Delay:           " << avgDelayMs << " ms" << std::endl;
    std::cout << "Collision Rate:      " << collisionRate << " %" << std::endl;

    // CSV出力
    std::string csvPath = "result_csv/" + config.outputFile;
    std::ofstream csv(csvPath, std::ios::app);
    
    if (csv.tellp() == 0) {
        csv << "Stations,Radius(m),Load(Mbps),PktSize,UseTCP,RateCtrl,MaxAmpdu,RtsCtsTh,"
            << "Utilization(%),Throughput(Mbps),LossRate(%),AvgDelay(ms),CollisionRate(%),"
            << "RetransCount,TotalMacTx,RetransRate(%),ApTxPower(dBm),AggRatio," // 項目追加
            << "AvgRSSI(dBm),AvgSNR(dB)" << std::endl;
    }
    
    csv << config.nStations << "," 
        << config.radius << "," 
        << (config.nHeavyUsers * config.heavyUserRate) << "," 
        << config.packetSize << ","
        << (config.useTcp ? "TCP" : "UDP") << ","
        << (config.useMinstrel ? "Auto" : "Fixed") << ","
        << config.maxAmpduSize << ","
        << config.rtsCtsThreshold << ","
        << channelUtil << ","
        << totalThroughput << ","
        << packetLossRate << "," 
        << avgDelayMs << ","     
        << collisionRate << ","
        << g_macTxFailed << ","     // 再送回数
        << g_macTxAttempts << ","   // 総送信回数
        << retransRate << ","       // 再送率
        << config.txPower << ","    // AP送信電力
        << aggregationRatio << ","
        << avgRssi << ","
        << avgSnr << std::endl;

    Simulator::Destroy();
    return 0;
}