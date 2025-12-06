/*
 * ns-3 無線LAN チャネル使用率シミュレーション (修正版v3)
 * 対応: IEEE 802.11ac, A-MPDU制御, レート制御切替, TCP/UDP, 衝突率測定
 * 追加: パケットロス率, 平均遅延, 配置半径の出力
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
uint64_t g_totalBusyTime = 0;       // チャネルBusy時間 (ns)
uint64_t g_phyTxFrames = 0;         // 物理層で送信されたフレーム数 (集約後)
uint64_t g_macTxAttempts = 0;       // MAC層での送信試行回数
uint64_t g_macTxFailed = 0;         // MAC層での送信失敗回数 (ACK未受信など)

// --- シミュレーション設定構造体 ---
struct SimulationConfig {
    uint32_t nStations;
    uint32_t heavyUserPercentage;
    uint32_t nHeavyUsers;
    uint32_t nLightUsers;

    double radius;          // 配置半径 (m) [YAML設定可能]
    std::string outputFile;
    uint32_t heavyUserRate; // Mbps
    uint32_t lightUserRate; // Mbps
    uint32_t packetSize;    // Byte
    double simulationTime;  // sec

    // === 追加機能用パラメータ ===
    bool useTcp;            // true: TCP, false: UDP
    bool useMinstrel;       // true: Minstrel(自動), false: Constant(固定)
    uint32_t maxAmpduSize;  // A-MPDU最大サイズ (Byte) 0で無効化
    uint32_t rtsCtsThreshold; // RTS/CTS閾値 (Byte)
    // ==========================

    bool enableTxtOutput;
    bool enableNetAnim;
    bool verbose;
};

// --- コールバック関数群 ---

// PHY状態変化 (Busy時間計測)
void PhyStateChangeCallback(std::string context, Time start, Time duration, WifiPhyState state) {
    if (state == WifiPhyState::TX || state == WifiPhyState::RX || state == WifiPhyState::CCA_BUSY) {
        g_totalBusyTime += duration.GetNanoSeconds();
    }
}

// PHY送信開始 (集約後の物理フレーム数カウント)
void PhyTxBeginCallback(std::string context, Ptr<const Packet> p, double txPowerW) {
    g_phyTxFrames++;
}

// MAC送信失敗 (衝突数の近似計測: ACKが返ってこなかった回数)
void MacTxDataFailedCallback(std::string context, Mac48Address addr) {
    g_macTxFailed++;
}

// MAC送信試行 (衝突率の分母)
void MacTxDataCallback(std::string context, Ptr<const Packet> p) {
    g_macTxAttempts++;
}


// チャネル使用率計算
double CalculateChannelUtilization(double simulationTimeSec) {
    if (simulationTimeSec <= 0) return 0.0;
    double totalTimeNano = simulationTimeSec * 1000000000.0;
    return (double)g_totalBusyTime / totalTimeNano * 100.0;
}

// タイムスタンプ生成
std::string GenerateTimestamp() {
    time_t now = time(0);
    struct tm tstruct = *localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tstruct);
    return std::string(buf);
}

// YAML読み込み
SimulationConfig LoadConfigFromYAML(const std::string& configFile) {
    SimulationConfig config;
    try {
        YAML::Node yamlConfig = YAML::LoadFile(configFile);
        
        config.nStations = yamlConfig["nStations"].as<uint32_t>();
        config.heavyUserPercentage = yamlConfig["heavyUserPercentage"].as<uint32_t>();
        
        config.nHeavyUsers = (config.nStations * config.heavyUserPercentage) / 100;
        config.nLightUsers = config.nStations - config.nHeavyUsers;

        config.radius = yamlConfig["radius"].as<double>(); // ここで半径を読み込み
        config.outputFile = yamlConfig["outputFile"].as<std::string>();
        config.heavyUserRate = yamlConfig["heavyUserRate"].as<uint32_t>();
        config.lightUserRate = yamlConfig["lightUserRate"].as<uint32_t>();
        config.packetSize = yamlConfig["packetSize"].as<uint32_t>();
        
        // 追加パラメータ (デフォルト値付き)
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

// デフォルト設定ファイル生成
void GenerateDefaultConfig(const std::string& filename) {
    std::ofstream out(filename);
    out << "# 実験計画対応版 設定ファイル" << std::endl;
    out << "nStations: 10" << std::endl;
    out << "heavyUserPercentage: 100" << std::endl;
    out << "radius: 10.0             # 配置半径(m)" << std::endl;
    out << "outputFile: \"experiment_result.csv\"" << std::endl;
    out << "heavyUserRate: 10" << std::endl;
    out << "lightUserRate: 2" << std::endl;
    out << "packetSize: 1500" << std::endl;
    out << "simulationTime: 10.0" << std::endl;
    out << std::endl;
    out << "# --- 実験条件スイッチ ---" << std::endl;
    out << "useTcp: false              # trueならTCP, falseならUDP" << std::endl;
    out << "useMinstrel: false         # trueなら自動レート制御, falseなら固定(Mcs8)" << std::endl;
    out << "maxAmpduSize: 65535        # A-MPDU最大サイズ(Byte). 0で無効化" << std::endl;
    out << "rtsCtsThreshold: 65535     # RTS/CTS閾値. 小さくするとRTS有効化" << std::endl;
    out << std::endl;
    out << "enableTxtOutput: true" << std::endl;
    out << "enableNetAnim: false" << std::endl;
    out << "verbose: false" << std::endl;
    out.close();
    std::cout << "Generated: " << filename << std::endl;
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

    // ノード作成
    NodeContainer wifiApNode;
    wifiApNode.Create(1);
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(config.nStations);

    // Wi-Fi設定 (11ac VHT)
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211ac);

    // レート制御の設定
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

    WifiMacHelper mac;
    Ssid ssid = Ssid("ns3-research");

    // APの設定
    mac.SetType("ns3::ApWifiMac", 
                "Ssid", SsidValue(ssid),
                "BE_MaxAmpduSize", UintegerValue(config.maxAmpduSize)); 
    NetDeviceContainer apDevice = wifi.Install(phy, mac, wifiApNode);

    // STAの設定
    mac.SetType("ns3::StaWifiMac", 
                "Ssid", SsidValue(ssid), 
                "ActiveProbing", BooleanValue(false),
                "BE_MaxAmpduSize", UintegerValue(config.maxAmpduSize)); 
    NetDeviceContainer staDevices = wifi.Install(phy, mac, wifiStaNodes);

    // RTS/CTS閾値の設定
    for (uint32_t i = 0; i < apDevice.GetN(); ++i) {
        Ptr<WifiNetDevice> dev = DynamicCast<WifiNetDevice>(apDevice.Get(i));
        dev->GetRemoteStationManager()->SetAttribute("RtsCtsThreshold", UintegerValue(config.rtsCtsThreshold));
    }
    for (uint32_t i = 0; i < staDevices.GetN(); ++i) {
        Ptr<WifiNetDevice> dev = DynamicCast<WifiNetDevice>(staDevices.Get(i));
        dev->GetRemoteStationManager()->SetAttribute("RtsCtsThreshold", UintegerValue(config.rtsCtsThreshold));
    }

    // 移動モデル (円形配置: 半径 radius を使用)
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

    // IPスタック
    InternetStackHelper stack;
    stack.Install(wifiApNode);
    stack.Install(wifiStaNodes);
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer apInterface = address.Assign(apDevice);
    Ipv4InterfaceContainer staInterfaces = address.Assign(staDevices);

    // アプリケーション
    ApplicationContainer serverApps, clientApps;
    uint16_t port = 9000;

    for (uint32_t i = 0; i < config.nStations; ++i) {
        uint32_t dataRateMbps = (i < config.nHeavyUsers) ? config.heavyUserRate : config.lightUserRate;
        Address serverAddress(InetSocketAddress(apInterface.GetAddress(0), port + i));

        if (config.useTcp) {
            PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port + i));
            serverApps.Add(packetSinkHelper.Install(wifiApNode.Get(0)));

            BulkSendHelper source("ns3::TcpSocketFactory", serverAddress);
            source.SetAttribute("MaxBytes", UintegerValue(0)); 
            clientApps.Add(source.Install(wifiStaNodes.Get(i)));
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

    // トレース設定
    Config::Connect("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/State/State",
                    MakeCallback(&PhyStateChangeCallback));
    Config::Connect("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/PhyTxBegin",
                    MakeCallback(&PhyTxBeginCallback));
    for(uint32_t i=0; i<config.nStations; i++){
        std::stringstream path;
        path << "/NodeList/" << (i+1) << "/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/MacTxDataFailed";
        Config::Connect(path.str(), MakeCallback(&MacTxDataFailedCallback));
        
        std::stringstream path2;
        path2 << "/NodeList/" << (i+1) << "/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx";
        Config::Connect(path2.str(), MakeCallback(&MacTxDataCallback));
    }

    // FlowMonitor
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // アニメーション (エラー修正済み)
    AnimationInterface *anim = nullptr;
    if (config.enableNetAnim) {
        std::string animFile = "results/" + GenerateTimestamp() + "_anim.xml";
        anim = new AnimationInterface(animFile);
    }

    NS_LOG_INFO("Simulating...");
    Simulator::Stop(Seconds(config.simulationTime + 0.1));
    Simulator::Run();

    // --- 結果集計 ---
    double channelUtil = CalculateChannelUtilization(config.simulationTime);
    double collisionRate = (g_macTxAttempts > 0) ? (double)g_macTxFailed / (g_macTxAttempts + (double)g_macTxFailed ) * 100.0 : 0.0;
    
    // スループット, 遅延, ロス率
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

    // 平均計算
    double avgDelayMs = (totalRxPackets > 0) ? (totalDelaySec / totalRxPackets) * 1000.0 : 0.0;
    double packetLossRate = (totalTxPackets > 0) ? (1.0 - (double)totalRxPackets / totalTxPackets) * 100.0 : 0.0;
    
    // 集約効率
    double aggregationRatio = (g_phyTxFrames > 0) ? (double)totalRxPackets / g_phyTxFrames : 0.0;

    // コンソール出力
    std::cout << "=== Result ===" << std::endl;
    std::cout << "Stations:            " << config.nStations << std::endl;
    std::cout << "Radius:              " << config.radius << " m" << std::endl;
    std::cout << "Channel Utilization: " << channelUtil << " %" << std::endl;
    std::cout << "Total Throughput:    " << totalThroughput << " Mbps" << std::endl;
    std::cout << "Packet Loss Rate:    " << packetLossRate << " %" << std::endl;
    std::cout << "Avg Delay:           " << avgDelayMs << " ms" << std::endl;
    std::cout << "Collision Rate:      " << collisionRate << " %" << std::endl;

    // CSV出力
    std::string csvPath = "result_csv/" + config.outputFile;
    std::ofstream csv(csvPath, std::ios::app);
    
    // ヘッダ (Loss, Delay, Radiusを追加)
    if (csv.tellp() == 0) {
        csv << "Stations,Radius(m),Load(Mbps),PktSize,UseTCP,RateCtrl,MaxAmpdu,RtsCtsTh,"
            << "Utilization(%),Throughput(Mbps),LossRate(%),AvgDelay(ms),CollisionRate(%),AggRatio" << std::endl;
    }
    
    csv << config.nStations << "," 
        << config.radius << "," // 配置半径を追加
        << (config.nHeavyUsers * config.heavyUserRate) << "," 
        << config.packetSize << ","
        << (config.useTcp ? "TCP" : "UDP") << ","
        << (config.useMinstrel ? "Auto" : "Fixed") << ","
        << config.maxAmpduSize << ","
        << config.rtsCtsThreshold << ","
        << channelUtil << ","
        << totalThroughput << ","
        << packetLossRate << "," // パケットロス率
        << avgDelayMs << ","     // 平均遅延
        << collisionRate << ","
        << aggregationRatio << std::endl;

    if (anim) delete anim;
    Simulator::Destroy();
    return 0;
}