/*
 * ns-3 無線LAN チャネル使用率シミュレーション (修正版v5)
 * 
 * 変更点:
 * 1. 衝突率(Collision Rate)の定義変更:
 *    計算式 = (全ノードの干渉・SINRによるドロップ回数) / (全ノードの物理層送信回数)
 *    ※ APと全端末(STA)の合計値で算出します。
 * 
 * 2. デフォルト設定の最適化:
 *    - RateControl: Minstrel (自動)
 *    - A-MPDU: 65535 (有効)
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
uint64_t g_totalBusyTime = 0;        // チャネルBusy時間 (ns)

// 衝突率計算用
uint64_t g_phyTxTotal = 0;           // 分母: 物理層での総送信回数 (全ノード)
uint64_t g_phyRxDropCollision = 0;   // 分子: 干渉(Interference)やSINR不足によるドロップ数 (全ノード)

// 集約効率計算用 (送信フレーム数)
uint64_t g_phyTxFrames = 0;          

// --- シミュレーション設定構造体 ---
struct SimulationConfig {
    uint32_t nStations;
    uint32_t heavyUserPercentage;
    uint32_t nHeavyUsers;
    uint32_t nLightUsers;

    double radius;          // 配置半径 (m)
    std::string outputFile;
    uint32_t heavyUserRate; // Mbps
    uint32_t lightUserRate; // Mbps
    uint32_t packetSize;    // Byte
    double simulationTime;  // sec

    bool useTcp;            
    bool useMinstrel;       
    uint32_t maxAmpduSize;  
    uint32_t rtsCtsThreshold; 

    bool enableTxtOutput;
    bool enableNetAnim;
    bool verbose;
};

// --- コールバック関数群 ---

// 1. チャネル使用率計測 (APのPHY状態監視)
void PhyStateChangeCallback(std::string context, Time start, Time duration, WifiPhyState state) {
    // TX(送信中), RX(受信中), CCA_BUSY(他干渉検知中) の時間を加算
    if (state == WifiPhyState::TX || state == WifiPhyState::RX || state == WifiPhyState::CCA_BUSY) {
        g_totalBusyTime += duration.GetNanoSeconds();
    }
}

// 2. 総送信回数カウント (衝突率の分母)
// 全ノード(AP+STA)が物理層で送信を開始した回数
void PhyTxBeginCallback(std::string context, Ptr<const Packet> p, double txPowerW) {
    g_phyTxTotal++;
    g_phyTxFrames++; // 集約効率計算用にも使用
}

// 3. 衝突回数カウント (衝突率の分子)
// 全ノード(AP+STA)でパケットドロップが発生した際、その理由が「衝突」関連か判定
void PhyRxDropCallback(std::string context, Ptr<const Packet> p, WifiPhyRxfailureReason reason) {
    // DROP_INTERFERENCE: 他の信号と重なってプリアンブル検知後に失敗
    // DROP_SINR: 信号対雑音干渉比が悪くて失敗 (主に衝突が原因)
    if (reason == WifiPhyRxfailureReason::DROP_INTERFERENCE || 
        reason == WifiPhyRxfailureReason::DROP_SINR) {
        g_phyRxDropCollision++;
    }
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

        config.radius = yamlConfig["radius"].as<double>();
        config.outputFile = yamlConfig["outputFile"].as<std::string>();
        config.heavyUserRate = yamlConfig["heavyUserRate"].as<uint32_t>();
        config.lightUserRate = yamlConfig["lightUserRate"].as<uint32_t>();
        config.packetSize = yamlConfig["packetSize"].as<uint32_t>();
        
        // 追加パラメータ (デフォルト値を推奨設定に変更)
        config.useTcp = yamlConfig["useTcp"].as<bool>(false);
        config.useMinstrel = yamlConfig["useMinstrel"].as<bool>(true);      // 推奨: true (Auto)
        config.maxAmpduSize = yamlConfig["maxAmpduSize"].as<uint32_t>(65535); // 推奨: 65535 (Enable Aggregation)
        config.rtsCtsThreshold = yamlConfig["rtsCtsThreshold"].as<uint32_t>(65535); 

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
    out << "# 実験計画対応版 設定ファイル (Collision Fix)" << std::endl;
    out << "nStations: 10" << std::endl;
    out << "heavyUserPercentage: 100" << std::endl;
    out << "radius: 10.0             # 配置半径(m)" << std::endl;
    out << "outputFile: \"experiment_result.csv\"" << std::endl;
    out << "heavyUserRate: 50" << std::endl;
    out << "lightUserRate: 2" << std::endl;
    out << "packetSize: 1500" << std::endl;
    out << "simulationTime: 10.0" << std::endl;
    out << std::endl;
    out << "# --- 実験条件スイッチ ---" << std::endl;
    out << "useTcp: false              # trueならTCP, falseならUDP" << std::endl;
    out << "useMinstrel: true          # true:Minstrl(Auto/推奨), false:Constant(Fixed)" << std::endl;
    out << "maxAmpduSize: 65535        # A-MPDU最大サイズ(Byte). 0で無効化, 65535で有効化" << std::endl;
    out << "rtsCtsThreshold: 65535     # RTS/CTS閾値. 65535で無効化" << std::endl;
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

    // RTS/CTSの閾値の設定
    for (uint32_t i = 0; i < apDevice.GetN(); ++i) {
        Ptr<WifiNetDevice> dev = DynamicCast<WifiNetDevice>(apDevice.Get(i));
        dev->GetRemoteStationManager()->SetAttribute("RtsCtsThreshold", UintegerValue(config.rtsCtsThreshold));
    }
    for (uint32_t i = 0; i < staDevices.GetN(); ++i) {
        Ptr<WifiNetDevice> dev = DynamicCast<WifiNetDevice>(staDevices.Get(i));
        dev->GetRemoteStationManager()->SetAttribute("RtsCtsThreshold", UintegerValue(config.rtsCtsThreshold));
    }

    // 移動モデル (円形配置)
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

    // --- トレース設定 (修正箇所) ---

    // 1. チャネル使用率 (APの状態のみを監視)
    //    APのコンテキストでChannelがBusyなら、そのセルのChannelはBusyとみなす
    Config::Connect("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/State/State",
                    MakeCallback(&PhyStateChangeCallback));
    
    // 2. 総送信回数 (全ノードを対象: wildcardを使用)
    //    "/NodeList/*" でAPもSTAもすべて含む
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/PhyTxBegin",
                    MakeCallback(&PhyTxBeginCallback));

    // 3. 衝突(ドロップ)回数 (全ノードを対象)
    //    誰かが受信に失敗(干渉)したらカウント
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/PhyRxDrop",
                    MakeCallback(&PhyRxDropCallback));


    // FlowMonitor
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // アニメーション
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
    
    // 衝突率の計算 (Collision Rate)
    // 定義: 干渉・SINRによる全ドロップ数 / 全物理送信回数
    double collisionRate = (g_phyTxTotal > 0) ? (double)g_phyRxDropCollision / g_phyTxTotal * 100.0 : 0.0;
    
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

    double avgDelayMs = (totalRxPackets > 0) ? (totalDelaySec / totalRxPackets) * 1000.0 : 0.0;
    double packetLossRate = (totalTxPackets > 0) ? (1.0 - (double)totalRxPackets / totalTxPackets) * 100.0 : 0.0;
    double aggregationRatio = (g_phyTxFrames > 0) ? (double)totalRxPackets / g_phyTxFrames : 0.0;

    // コンソール出力
    std::cout << "=== Result ===" << std::endl;
    std::cout << "Stations:            " << config.nStations << std::endl;
    std::cout << "Channel Utilization: " << channelUtil << " %" << std::endl;
    std::cout << "Total Throughput:    " << totalThroughput << " Mbps" << std::endl;
    std::cout << "Packet Loss Rate:    " << packetLossRate << " %" << std::endl;
    std::cout << "Collision Rate:      " << collisionRate << " % (Physical Interference)" << std::endl;

    // CSV出力
    std::string csvPath = "result_csv/" + config.outputFile;
    std::ofstream csv(csvPath, std::ios::app);
    
    if (csv.tellp() == 0) {
        csv << "Stations,Radius(m),Load(Mbps),PktSize,UseTCP,RateCtrl,MaxAmpdu,RtsCtsTh,"
            << "Utilization(%),Throughput(Mbps),LossRate(%),AvgDelay(ms),CollisionRate(%),AggRatio" << std::endl;
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
        << collisionRate << "," // 物理層での衝突率
        << aggregationRatio << std::endl;

    if (anim) delete anim;
    Simulator::Destroy();
    return 0;
}