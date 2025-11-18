# ns-3.45 無線LANシミュレーション環境

ns-3.45を使用した無線LANチャネル使用率シミュレーションの完全セットアップガイドです。

---

## 📋 目次

1. [このプロジェクトについて](#このプロジェクトについて)
2. [準備するもの](#準備するもの)
3. [セットアップ手順](#セットアップ手順)
4. [シミュレーションの実行](#シミュレーションの実行)
5. [便利なショートカット設定](#便利なショートカット設定)
6. [設定ファイルの編集](#設定ファイルの編集)
7. [結果の確認方法](#結果の確認方法)
8. [よくあるエラーと解決方法](#よくあるエラーと解決方法)

---

## 🎯 このプロジェクトについて

このプロジェクトでは、ns-3.45シミュレータを使って以下のことができます：

- **無線LANのチャネル使用率測定**: APと複数のクライアント端末の通信をシミュレート
- **Heavy/Lightユーザの混在環境**: データ使用量の異なるユーザが混在する環境を再現
- **パフォーマンス評価**: スループット、遅延、パケット損失率などを測定

---

## 💻 準備するもの

### 必要な環境
- **macOS**（Intel Mac または Apple Silicon Mac）
- インターネット接続
- 約2GBの空きディスク容量

### インストールが必要なソフトウェア

以下のコマンドを順番に実行してください：
```bash
# 1. Homebrewがインストールされているか確認
which brew

# もしHomebrewがインストールされていなければ、以下を実行
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 2. 必要なツールをインストール
brew install python@3.13
brew install yaml-cpp
brew install cmake
```

**重要**: Python 3.14は使用しないでください。Python 3.13を使用してください。

---

## 🔧 セットアップ手順

### ステップ1: ディレクトリ構造を確認

プロジェクトは以下のような構造になっています：
```
ns-allinone-3.45/
└── ns-3.45/                          ← この中で作業します
    ├── scratch/                      ← シミュレーションコードを置く場所
    │   ├── scratch-simulator.cc      ← テスト用の簡単なシミュレータ
    │   └── channelutilization/       ← チャネル使用率シミュレーション
    │       ├── channelutilization.cc
    │       ├── config.yaml
    │       └── CMakeLists.txt
    └── cmake-cache/                  ← ビルド後にここに実行ファイルが作られます
```

### ステップ2: channelutilizationディレクトリを作成

ターミナルを開いて、以下を実行してください：
```bash
# ns-3.45ディレクトリに移動（パスは自分の環境に合わせて変更）
cd /Users/あなたのユーザ名/Desktop/ns-allinone-3.45/ns-3.45

# channelutilizationディレクトリを作成
mkdir -p scratch/channelutilization

# もしchannelutilization.ccがすでにscratch/にあれば移動
# mv scratch/channelutilization.cc scratch/channelutilization/
```

### ステップ3: CMakeLists.txtを作成

`scratch/channelutilization/CMakeLists.txt` というファイルを作成します：
```bash
cat > scratch/channelutilization/CMakeLists.txt << 'CMAKEFILE'
# yaml-cppライブラリの場所を指定
set(YAML_CPP_INCLUDE_DIR "/opt/homebrew/Cellar/yaml-cpp/0.8.0/include")
set(YAML_CPP_LIBRARY "/opt/homebrew/Cellar/yaml-cpp/0.8.0/lib/libyaml-cpp.0.8.0.dylib")

# ソースファイルを取得
file(GLOB scratch_sources CONFIGURE_DEPENDS *.cc)

# 実行ファイルをビルド
build_exec(
  EXECNAME channelutilization
  EXECNAME_PREFIX scratch_
  SOURCE_FILES "${scratch_sources}"
  LIBRARIES_TO_LINK "${ns3-libs}" "${ns3-contrib-libs}" "${YAML_CPP_LIBRARY}"
  EXECUTABLE_DIRECTORY_PATH ${CMAKE_CURRENT_BINARY_DIR}/
)

# インクルードディレクトリを追加
target_include_directories(scratch_channelutilization PRIVATE ${YAML_CPP_INCLUDE_DIR})
CMAKEFILE
```

**注意**: yaml-cppのバージョンが違う場合は、以下のコマンドで確認して、パスを修正してください：
```bash
brew list yaml-cpp
```

### ステップ4: プロジェクトをビルド
```bash
# ns-3.45ディレクトリにいることを確認
pwd
# 出力: /Users/あなたのユーザ名/Desktop/ns-allinone-3.45/ns-3.45

# Python 3.13を使ってビルド設定
python3.13 ./ns3 configure --enable-examples --enable-tests

# ビルド実行（5-10分かかります）
python3.13 ./ns3 build
```

ビルドが完了したら、以下のメッセージが表示されます：
```
Finished executing the following commands:
...
```

---

## 🚀 シミュレーションの実行

### 基本的な実行方法

#### 1. テスト用シミュレータを実行（動作確認）
```bash
./cmake-cache/scratch/ns3.45-scratch-simulator-default
```

**期待される出力**:
```
Scratch Simulator
```

この出力が表示されればOKです！

#### 2. 設定ファイルを生成
```bash
./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default --generate-config=true

# 生成されたconfig.yamlを移動
mv config.yaml scratch/channelutilization/
```

#### 3. チャネル使用率シミュレーションを実行
```bash
./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default --config=scratch/channelutilization/config.yaml
```

---

## ⚡ 便利なショートカット設定

毎回長いコマンドを入力するのは大変なので、ショートカットを設定しましょう。

### ショートカットの登録

ターミナルで以下を実行してください：
```bash
cat >> ~/.zshrc << 'EOF'

# ========================================
# ns-3 便利コマンド
# ========================================

# ns-3ディレクトリに素早く移動
alias ns3-cd='cd /Users/あなたのユーザ名/Desktop/ns-allinone-3.45/ns-3.45'

# チャネル使用率シミュレーションを実行
run-channel() {
    cd /Users/あなたのユーザ名/Desktop/ns-allinone-3.45/ns-3.45
    if [ $# -eq 0 ]; then
        ./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default --config=scratch/channelutilization/config.yaml
    else
        ./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default "$@"
    fi
    cd - > /dev/null
}

# テストシミュレータを実行
run-simulator() {
    cd /Users/あなたのユーザ名/Desktop/ns-allinone-3.45/ns-3.45
    ./cmake-cache/scratch/ns3.45-scratch-simulator-default "$@"
    cd - > /dev/null
}

# プロジェクトを再ビルド
ns3-build() {
    cd /Users/あなたのユーザ名/Desktop/ns-allinone-3.45/ns-3.45
    python3.13 ./ns3 build
    cd - > /dev/null
}
EOF

# 設定を読み込み
source ~/.zshrc
```

**重要**: `/Users/あなたのユーザ名/Desktop/ns-allinone-3.45/ns-3.45` の部分を、
自分のns-3.45があるパスに置き換えてください。

### ショートカットの使い方

設定後は、どのディレクトリからでも以下のコマンドが使えます：
```bash
# シミュレーションを実行
run-channel

# 別の設定ファイルで実行
run-channel --config=別の設定.yaml

# テストシミュレータを実行
run-simulator

# プロジェクトをビルド
ns3-build

# ns-3ディレクトリに移動
ns3-cd
```

---

## ⚙️ 設定ファイルの編集

### 設定ファイルを開く
```bash
# nanoエディタで開く（初心者向け）
nano scratch/channelutilization/config.yaml

# または、VS Codeで開く
code scratch/channelutilization/config.yaml
```

**nanoエディタの使い方**:
- 編集する: 矢印キーでカーソルを移動して編集
- 保存する: `Ctrl + X` → `Y` → `Enter`

### 設定項目の説明
```yaml
# 総ユーザ数（APに接続する端末の数）
nStations: 10

# 重ユーザ数（データをたくさん使うユーザ）
nHeavyUsers: 7

# 軽ユーザ数（データをあまり使わないユーザ）
nLightUsers: 3

# 重ユーザの割合（%）
# 計算式: (nHeavyUsers / nStations) × 100
heavyUserPercentage: 70

# APからの距離（メートル）
radius: 7.5

# 結果を保存するCSVファイル名
outputFile: "channel_utilization_results.csv"

# 重ユーザのデータ速度（Mbps）
heavyUserRate: 50

# 軽ユーザのデータ速度（Mbps）
lightUserRate: 20

# パケットサイズ（バイト）
packetSize: 1500

# シミュレーション時間（秒）
simulationTime: 10.0

# 詳細な結果をテキストファイルに保存するか
enableTxtOutput: true

# NetAnimアニメーションを生成するか（falseを推奨）
enableNetAnim: false

# 詳細なログを表示するか
verbose: false
```

### 設定のコツ

✅ **必ず守ること**:
- `nStations = nHeavyUsers + nLightUsers` にする
- 例: `nStations: 10`, `nHeavyUsers: 7`, `nLightUsers: 3`

⚠️ **注意点**:
- `enableNetAnim: true` にすると、パケット数が多い時にエラーが出ることがあります
- 初めての場合は `enableNetAnim: false` のままにしてください

### 設定例：10ユーザ、Heavy 70%
```yaml
nStations: 10
nHeavyUsers: 7
nLightUsers: 3
heavyUserPercentage: 70
radius: 7.5
outputFile: "test_10users_70heavy.csv"
heavyUserRate: 50
lightUserRate: 20
packetSize: 1500
simulationTime: 10.0
enableTxtOutput: true
enableNetAnim: false
verbose: false
```

---

## 📊 結果の確認方法

### 生成されるファイル

シミュレーション実行後、以下のファイルが作成されます：
```
ns-3.45/
├── results/                          ← 詳細な結果
│   └── channelutilization_20251118_143025/
│       └── results_n10_h70.txt       ← テキスト形式の詳細結果
└── result_csv/                       ← CSV形式の結果
    └── channel_utilization_results.csv
```

### CSV結果を見る
```bash
# CSVファイルの内容を表示
cat result_csv/channel_utilization_results.csv

# 最新の結果だけ表示
tail -1 result_csv/channel_utilization_results.csv
```

**CSV形式の例**:
```
クライアント数,重ユーザ数,軽ユーザ数,重ユーザ割合,配置半径,シミュレーション時間,チャネル使用率,平均スループット,平均遅延,パケット損失率,タイムスタンプ
10,7,3,70,7.5,10.0,45.2,38.5,12.3,0.5,20251118_143025_t10.0s
```

### テキスト結果を見る
```bash
# 最新の結果ディレクトリを確認
ls -lt results/ | head -5

# 結果ファイルを表示
cat results/channelutilization_*/results_*.txt
```

**テキスト結果の例**:
```
========================================
ns-3 無線LANチャネル使用率シミュレーション結果
========================================

[シミュレーションパラメータ]
総端末数: 10
Heavyユーザ数: 7 (70%)
Lightユーザ数: 3 (30%)
...

[チャネル使用率]
チャネル使用率: 45.2 %
...

[性能指標]
平均スループット: 38.5 Mbps
平均遅延: 12.3 ms
パケット損失率: 0.5 %
...
```

---

## 🔧 よくあるエラーと解決方法

### エラー1: `./ns3` が動かない

**エラーメッセージ**:
```
ValueError: action 'store_true' is not valid for positional arguments
```

**原因**: Python 3.14を使っている

**解決方法**:
```bash
# Python 3.13をインストール
brew install python@3.13

# Python 3.13を使ってビルド
python3.13 ./ns3 build
```

---

### エラー2: yaml-cppが見つからない

**エラーメッセージ**:
```
ld: library 'yaml-cpp' not found
```

**解決方法**:
```bash
# yaml-cppをインストール
brew install yaml-cpp

# インストール場所を確認
brew list yaml-cpp

# CMakeLists.txtのパスを確認・修正
nano scratch/channelutilization/CMakeLists.txt
```

---

### エラー3: config.yamlが見つからない

**エラーメッセージ**:
```
YAML読み込みエラー: bad file: config.yaml
```

**解決方法**:
```bash
# 設定ファイルを生成
./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default --generate-config=true

# 正しい場所に移動
mv config.yaml scratch/channelutilization/

# 正しいパスを指定して実行
run-channel
```

---

### エラー4: パケット数が多すぎる

**エラーメッセージ**:
```
Max Packets per trace file exceeded
```

**原因**: NetAnimが有効になっている

**解決方法**:
```bash
# config.yamlを編集
nano scratch/channelutilization/config.yaml

# 以下の行を変更
enableNetAnim: false
```

---

### エラー5: ビルドが失敗する（uart-net-device）

**エラーメッセージ**:
```
Undefined symbols for architecture arm64
...uart-net-device...
```

**解決方法**:
```bash
# contrib/uart-net-deviceを無効化
cd contrib
mv uart-net-device uart-net-device.disabled

# 元のディレクトリに戻る
cd ..

# 再ビルド
python3.13 ./ns3 clean
python3.13 ./ns3 build
```

---

### 警告: unused variable

**警告メッセージ**:
```
warning: unused variable 't'
```

**これは何？**: これは単なる警告で、エラーではありません。

**対処**: 無視しても大丈夫です。シミュレーションは正常に動作します。

---

## ❓ よくある質問

### Q1: どのPythonバージョンを使えばいいですか？

**A**: Python 3.13を使ってください。Python 3.14は使わないでください。
```bash
# 今使っているPythonのバージョンを確認
python3 --version
python3.13 --version
python3.14 --version

# Python 3.13をインストール
brew install python@3.13
```

---

### Q2: 結果はどこに保存されますか？

**A**: 2つの場所に保存されます：

1. **詳細な結果**: `results/channelutilization_日付_時刻/`
2. **CSV結果**: `result_csv/`

---

### Q3: 異なる条件で複数回シミュレーションしたい

**A**: 設定ファイルを複数作成して、それぞれ実行してください：
```bash
# 設定ファイル1を作成
cp scratch/channelutilization/config.yaml scratch/channelutilization/config_scenario1.yaml

# 設定ファイル2を作成
cp scratch/channelutilization/config.yaml scratch/channelutilization/config_scenario2.yaml

# それぞれ編集してから実行
run-channel --config=scratch/channelutilization/config_scenario1.yaml
run-channel --config=scratch/channelutilization/config_scenario2.yaml
```

---

### Q4: シミュレーション結果をExcelで見たい

**A**: CSV結果ファイルをExcelで開けます：

1. Finderで `result_csv` フォルダを開く
2. `channel_utilization_*.csv` をダブルクリック
3. Excelまたはスプレッドシートで開く

---

### Q5: ビルドに時間がかかりすぎる

**A**: 初回ビルドは5-15分かかることがあります。

2回目以降は変更した部分だけビルドされるので、もっと速くなります。

---

## 📚 参考リンク

- [ns-3公式サイト](https://www.nsnam.org/)
- [ns-3チュートリアル](https://www.nsnam.org/docs/tutorial/html/)
- [ns-3ドキュメント](https://www.nsnam.org/documentation/)

---

## 📝 更新履歴

- **2025-11-18**: 初版作成
  - 環境構築からシミュレーション実行まで
  - トラブルシューティング追加
  - 初心者向けに詳しく解説
