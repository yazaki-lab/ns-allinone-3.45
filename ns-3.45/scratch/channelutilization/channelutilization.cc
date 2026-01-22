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

// ================================
// L1 (PHY層): 無線チャネル占有・物理送信
// ================================
uint64_t g_totalBusyTime = 0;   // 【L1】PHYがTX/RX/CCA_BUSYだった累積時間(ns)
uint64_t g_phyTxFrames = 0;     // 【L1】PHYレベルで送信開始したフレーム数
                               //      ※ACK, RTS/CTS, 再送も全て含む

// ================================
// L2 (MAC層): CSMA/CA・再送制御
// ================================
uint64_t g_macTxAttempts = 0;   // 【L2】MAC層での送信試行回数（初回+再送）
uint64_t g_macTxFailed = 0;     // 【L2】ACK未受信による再送発生回数

// ================================
// L1 (PHY層): 受信信号品質
// ================================
double g_totalRssi = 0.0;       // 【L1】受信信号強度(dBm)
double g_totalSnr = 0.0;        // 【L1】SNR(dB)
uint64_t g_rxSignalCount = 0;   // 【L1】受信サンプル数


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
    double txPower;         // AP送信電力 (dBm)
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

void PhyStateChangeCallback(std::string context,Time start,Time duration,WifiPhyState state)
{
    // 【L1】PHY層の状態変化
    // TX: 送信中
    // RX: 受信中
    // CCA_BUSY: 他局送信によるキャリア検知
    if (state == WifiPhyState::TX ||
        state == WifiPhyState::RX ||
        state == WifiPhyState::CCA_BUSY)
    {
        // 【L1】チャネルが占有されていた時間
        g_totalBusyTime += duration.GetNanoSeconds();
    }
}


void PhyTxBeginCallback(std::string context,
                        Ptr<const Packet> p,
                        double txPowerW)
{
    // 【L1】PHYレベルで「送信が始まった」回数
    // データ・ACK・RTS/CTS・再送を全て含む
    g_phyTxFrames++;
}

void MacTxDataFailedCallback(std::string context,
                             Mac48Address addr)
{
    // 【L2】MAC層でデータフレーム送信に失敗
    // → ACKが返らなかったため再送が発生
    g_macTxFailed++;
}

void MacTxDataCallback(std::string context,
                        Ptr<const Packet> p)
{
    // 【L2】MAC層での送信試行
    // 初回送信 + 再送の両方がカウントされる
    g_macTxAttempts++;
}


void MonitorSnifferRxCallback(std::string context,
                              Ptr<const Packet> packet,
                              uint16_t channelFreqMhz,
                              WifiTxVector txVector,
                              MpduInfo mpduInfo,
                              SignalNoiseDbm signalNoise,
                              uint16_t staId)
{
    // 【L1】PHYレベルでの受信信号情報
    // IP/UDP/TCPの概念は一切存在しない
    g_totalRssi += signalNoise.signal;
    g_totalSnr  += (signalNoise.signal - signalNoise.noise);
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
        config.txPower = yamlConfig["txPower"].as<double>(16.0206);
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
    out << "packetSize: 1460" << std::endl; // ※修正: MSSサイズに統一
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

    std::string configFile = "config.yaml";//標準ライブラリの文字列型を使用する．"config.yaml"という名前のYAMLファイルを指定(変数configFileは自由に変更可)
    bool generateConfig = false;//設定ファイルを作ってくれる変数．基本はoff

    CommandLine cmd;//コマンドラインからも設定を受け取れるようにする
    cmd.AddValue("config", "YAML config file", configFile);
    cmd.AddValue("generate-config", "Generate default config", generateConfig);
    cmd.Parse(argc, argv);

    if (generateConfig) {
        GenerateDefaultConfig(configFile);
        return 0;
    }

    SimulationConfig config = LoadConfigFromYAML(configFile);//YAMLファイルから設定を読み込む

    if (config.verbose) {
        LogComponentEnable("WifiChannelUtilizationSim", LOG_LEVEL_INFO);//ログ出力を有効化
    }

    NodeContainer wifiApNode;
    wifiApNode.Create(1);//APノードを1台作成
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(config.nStations);//stationノードを設定数だけ作成

    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211ax);//802.11axを使用

    if (config.useMinstrel) {//Minstrelを使用するかどうかの分岐
        wifi.SetRemoteStationManager("ns3::MinstrelHtWifiManager");//Minstrelを使用
    } else {
        wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",//固定レートを使用
                                     "DataMode", StringValue("VhtMcs8"),//送信モード
                                     "ControlMode", StringValue("VhtMcs0"));//制御モード
    }

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();//デフォルトのチャネルモデルを使用
    YansWifiPhyHelper phy;//PHYヘルパーを作成
    phy.SetChannel(channel.Create());//PHY層(L1)の設定

    // YAMLから読み込んだ送信電力を設定
    phy.Set("TxPowerStart", DoubleValue(config.txPower));
    phy.Set("TxPowerEnd", DoubleValue(config.txPower));

    WifiMacHelper mac;
    Ssid ssid = Ssid("ns3-research");//ssidを設定．シンプルにするため1つだけ

    mac.SetType("ns3::ApWifiMac",
                "Ssid", SsidValue(ssid),
                "BE_MaxAmpduSize", UintegerValue(config.maxAmpduSize));//AP用のMAC設定
    NetDeviceContainer apDevice = wifi.Install(phy, mac, wifiApNode);//APノードにデバイスをインストール

    mac.SetType("ns3::StaWifiMac",//STA用のMAC設定
                "Ssid", SsidValue(ssid),
                "ActiveProbing", BooleanValue(false),
                "BE_MaxAmpduSize", UintegerValue(config.maxAmpduSize));
    NetDeviceContainer staDevices = wifi.Install(phy, mac, wifiStaNodes);

    for (uint32_t i = 0; i < apDevice.GetN(); ++i) {//RtsCtsThresholdの設定．隠れ端末を防ぐ，必要に応じて有効化
        Ptr<WifiNetDevice> dev = DynamicCast<WifiNetDevice>(apDevice.Get(i));
        dev->GetRemoteStationManager()->SetAttribute("RtsCtsThreshold", UintegerValue(config.rtsCtsThreshold));
    }
    for (uint32_t i = 0; i < staDevices.GetN(); ++i) {
        Ptr<WifiNetDevice> dev = DynamicCast<WifiNetDevice>(staDevices.Get(i));
        dev->GetRemoteStationManager()->SetAttribute("RtsCtsThreshold", UintegerValue(config.rtsCtsThreshold));
    }

    MobilityHelper mobility;//ノードの位置設定
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(0.0, 0.0, 0.0)); // AP
    for (uint32_t i = 0; i < config.nStations; ++i) {
        double angle = (2.0 * M_PI * i) / config.nStations;
        positionAlloc->Add(Vector(config.radius * cos(angle), config.radius * sin(angle), 0.0));
    }
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(wifiApNode);//ここでノードを実際に設置する
    mobility.Install(wifiStaNodes);

    InternetStackHelper stack;
    stack.Install(wifiApNode);//TCP/IPプロトコルスタックをインストール
    stack.Install(wifiStaNodes);
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");//IPアドレスの設定
    Ipv4InterfaceContainer apInterface = address.Assign(apDevice);//APのIPアドレス割り当て．Assignが賢いので1つづつ順番に割り当ててくれる
    Ipv4InterfaceContainer staInterfaces = address.Assign(staDevices);

    ApplicationContainer serverApps, clientApps;
    uint16_t port = 9000;
    //【L7】アプリケーション層での送信レート指定
    for (uint32_t i = 0; i < config.nStations; ++i) {
        uint32_t dataRateMbps = (i < config.nHeavyUsers) ? config.heavyUserRate : config.lightUserRate;//重ユーザと軽ユーザでレートを分ける
        Address serverAddress(InetSocketAddress(apInterface.GetAddress(0), port + i));//サーバーアドレスの設定

        if (config.useTcp) {
            // 受信側 (Server: AP)
            PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port + i));
            //TCPのソケットを作成する，
            serverApps.Add(packetSinkHelper.Install(wifiApNode.Get(0)));

            // 送信側 (Client: STA)
            // OnOffHelperを使うと、TCPでも指定レート(DataRate)で送信しようと制御
            // 送信プロトコルがTCPの場合，アプリ側でずっと送信状態にしてもTCP側で制御される
            OnOffHelper onoff("ns3::TcpSocketFactory", serverAddress);

            // ずっとON(送信状態)にする設定
            onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
            onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

            // レートとパケットサイズの設定
            onoff.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(dataRateMbps) + "Mbps")));//yamlで指定した指定レートで送信
            onoff.SetAttribute("PacketSize", UintegerValue(config.packetSize));//yamlで指定したパケットサイズで送信

            clientApps.Add(onoff.Install(wifiStaNodes.Get(i)));
        } else {
            UdpServerHelper server(port + i);//UDPサーバーの設定
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
                    MakeCallback(&PhyStateChangeCallback));//AP(Node 0)のPHY状態変化を監視
    Config::Connect("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/PhyTxBegin",
                    MakeCallback(&PhyTxBeginCallback));//AP(Node 0)のPHY送信開始を監視
    Config::Connect("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/MonitorSnifferRx",
                    MakeCallback(&MonitorSnifferRxCallback));//AP(Node 0)の受信パケットを監視

    for(uint32_t i=0; i<config.nStations; i++){
        std::stringstream path;//STAノードのMAC送信試行・失敗を監視する文字列
        path << "/NodeList/" << (i+1) << "/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/MacTxDataFailed";
        //Nodeリストの (i+1)番目のPC にあるデバイスリストの中の
        // Wi-Fiデバイス型であるもののうち， 通信管理マネージャーの中にある
        //『送信失敗（MacTxDataFailed）』という名前の監視ポイント
        Config::Connect(path.str(), MakeCallback(&MacTxDataFailedCallback));//STA側の送信失敗もカウント対象に含める

        std::stringstream path2;//STAノードのMAC送信試行を監視
        path2 << "/NodeList/" << (i+1) << "/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTx";
        Config::Connect(path2.str(), MakeCallback(&MacTxDataCallback));//STA側の送信試行もカウント対象に含める
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

    //=============================================================
    //=============================================================
    //         結果集計
    //=============================================================
    //=============================================================
    double channelUtil = CalculateChannelUtilization(config.simulationTime);
    double collisionRate = (g_macTxAttempts > 0) ? (double)g_macTxFailed / (g_macTxAttempts + (double)g_macTxFailed ) * 100.0 : 0.0;

    // 再送率の計算
    double retransRate = (g_macTxAttempts > 0) ? (double)g_macTxFailed / g_macTxAttempts * 100.0 : 0.0;

    monitor->CheckForLostPackets();

    // フロー識別子の取得とポートフィルタリング準備
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

    double totalThroughput = 0.0;
    long totalRxPackets = 0;
    long totalTxPackets = 0;
    double totalDelaySec = 0.0;


    for (auto const &flow : stats) {
        // ポート番号によるフィルタリング (戻りACK除外)
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(flow.first);

        // デバッグ用: 9000番台以外のポートが混ざっていないか確認
        if (t.destinationPort < 9000) {
            std::cout << "Warning: Counting non-server port packet! Port: " << t.destinationPort << std::endl;
        }
        // 宛先ポートがサーバーポート(9000番台)の範囲内かチェック
        if (t.destinationPort >= 9000 && t.destinationPort < 9000 + config.nStations) {

            // データフローのみ集計
            totalThroughput += flow.second.rxBytes * 8.0 / 1e6 / config.simulationTime;

            totalRxPackets += flow.second.rxPackets;
            totalTxPackets += flow.second.txPackets;
            totalDelaySec += flow.second.delaySum.GetSeconds();
        }
    }

//     //=============================================================
//     //以下デバッグ用の追加情報
//     //=============================================================
//     double channelUtil = CalculateChannelUtilization(config.simulationTime);
//     double collisionRate = (g_macTxAttempts > 0) ? (double)g_macTxFailed / (g_macTxAttempts + (double)g_macTxFailed ) * 100.0 : 0.0;
//     double retransRate = (g_macTxAttempts > 0) ? (double)g_macTxFailed / g_macTxAttempts * 100.0 : 0.0;

//     monitor->CheckForLostPackets();

//     Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
//     std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

//     double totalThroughput = 0.0;
//     long totalRxPackets = 0;
//     long totalTxPackets = 0;
//     double totalDelaySec = 0.0;

//     // === 情報追加 ===
//     std::cout << "\n=== Flow Debug Information ===" << std::endl;
//     double expectedTotalRate = 0.0;
//     for (uint32_t i = 0; i < config.nStations; ++i) {
//         uint32_t rate = (i < config.nHeavyUsers) ? config.heavyUserRate : config.lightUserRate;
//         expectedTotalRate += rate;
//     }
//     std::cout << "Expected Total Rate: " << expectedTotalRate << " Mbps" << std::endl;
//     std::cout << "Simulation Time: " << config.simulationTime << " sec" << std::endl;
//     std::cout << "Client Start Time: 1.0 sec" << std::endl;
//     std::cout << "Actual Tx Duration: " << (config.simulationTime - 1.0) << " sec" << std::endl;

//     int flowIndex = 0;
//     for (auto const &flow : stats) {
//         Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(flow.first);

//         if (t.destinationPort >= 9000 && t.destinationPort < 9000 + config.nStations) {
//             flowIndex++;

//             double firstRx = flow.second.timeFirstRxPacket.GetSeconds();
//             double lastRx = flow.second.timeLastRxPacket.GetSeconds();
//             double flowDuration = lastRx - firstRx;

//             double flowThroughput = flow.second.rxBytes * 8.0 / 1e6 / flowDuration;

//             std::cout << "\nFlow " << flowIndex << " (Port " << t.destinationPort << "):" << std::endl;
//             std::cout << "  First Rx: " << firstRx << " sec" << std::endl;
//             std::cout << "  Last Rx:  " << lastRx << " sec" << std::endl;
//             std::cout << "  Duration: " << flowDuration << " sec" << std::endl;
//             std::cout << "  Rx Bytes: " << flow.second.rxBytes << std::endl;
//             std::cout << "  Rx Pkts:  " << flow.second.rxPackets << std::endl;
//             std::cout << "  Tx Pkts:  " << flow.second.txPackets << std::endl;
//             std::cout << "  Throughput (flow-based): " << flowThroughput << " Mbps" << std::endl;

//             // 全体の送信時間で計算した場合
//             double throughputBySimTime = flow.second.rxBytes * 8.0 / 1e6 / config.simulationTime;
//             double throughputByActualTx = flow.second.rxBytes * 8.0 / 1e6 / (config.simulationTime - 1.0);
//             std::cout << "  Throughput (sim-time):   " << throughputBySimTime << " Mbps" << std::endl;
//             std::cout << "  Throughput (actual-tx):  " << throughputByActualTx << " Mbps" << std::endl;

//             if (flowDuration <= 0.0) {
//                 std::cout << "  WARNING: Invalid flow duration!" << std::endl;
//                 continue;
//             }

//             totalThroughput += flowThroughput;
//             totalRxPackets += flow.second.rxPackets;
//             totalTxPackets += flow.second.txPackets;
//             totalDelaySec += flow.second.delaySum.GetSeconds();
//         }
//     }

// std::cout << "\n=== Throughput Comparison ===" << std::endl;
// std::cout << "Expected:  " << expectedTotalRate << " Mbps" << std::endl;
// std::cout << "Measured:  " << totalThroughput << " Mbps" << std::endl;
// std::cout << "Difference: " << (totalThroughput - expectedTotalRate) << " Mbps (" 
//           << ((totalThroughput - expectedTotalRate) / expectedTotalRate * 100.0) << " %)" << std::endl;

// 以降はコンソール出力とCSV出力を続ける
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
    std::cout << "Total Mac Tx:        " << g_macTxAttempts << " times" << std::endl;
    std::cout << "Retransmissions:     " << g_macTxFailed << " times" << std::endl;
    std::cout << "Retransmission Rate: " << retransRate << " %" << std::endl;
    std::cout << "AP Tx Power:         " << config.txPower << " dBm" << std::endl;
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