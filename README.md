<<<<<<< HEAD
This is **_ns-3-allinone_**, a repository with some scripts that bundle
ns-3's mainline source code with compatible
[App Store modules](https://apps.nsnam.org).

The mainline ns-3 release or development tree (ns-3-dev) only contains
the ns-3 project's maintained
modules (libraries) as found in the `src/` directory.  The `contrib/`
directory is empty in the project mainline, allowing users to later
download or clone extension modules to it.

In contrast, the ns-3 source code in this release contains several
additional contrib modules known to work with the release.  In
all other respects, this release should be identical with the main
tree release with the same number.

## Usage

If you have downloaded this as a source archive of a release, simply
recurse into the ns-3 directory and configure and build ns-3 as usual.
The build process will include all modules found in the contrib directory
that are compatible with your system (as detected by the `ns3` build
script).  If you are not interested in some of the contributed modules,
and want to shorten the compilation time, feel free to delete any such
subdirectories from your `contrib` directory.
 
If you have cloned ns-3-allinone.git, you can checkout the manifest
that corresponds to a particular release by checking out a tagged
branch such as follows.

By default, the `download.py` script will clone and checkout the latest
ns-3 release.

```shell
./download.py
```
After the above command succeeds, an `ns-3.45` directory will be
present containing the latest release, and within that directory's
contrib directory, several extension modules will be downloaded.
`download.py` reports on the extra modules that have been downloaded.

The script also can be used to download and check against ns-3-dev
as follows:

```shell
./download.py ns-3-dev
```

Note that using download.py with ns-3-dev may lead to compilation errors
if the contrib modules listed in `MANIFEST.md` have not been upgraded
to ns-3-dev compatibility, but any such modules could either be fixed locally
or else deleted if not of interest.  

ns-3.45 is the earliest such release that is supported; see
[History](#history) below for ns-3-allinone prior to ns-3.45.

## Documentation

The manifest of contributed modules can be found in [MANIFEST.md](MANIFEST.md).
This release does not package documentation of the contributed modules; please
visit the App Store page or the module's repository itself for such documentation.
 
## Scope and Limitations

The ns-3 mainline undergoes thorough CI testing of the build on many
systems and compiler versions, as well as documentation and code style
checks.  Contributed modules are not subjected to the same level of testing
or adherence to style or other conventions.  This distribution errs on the
side of inclusion of many modules so that users may learn about them,
with the downside that users may encounter compilation problems on some
systems.  The easiest fix is to delete any contrib modules that are
causing problems, unless of course you want to use those modules, in which
case you will need to fix that code by hand.

If users find a compatibility issue with a contributed module, please
file an issue on the upstream module's issue tracker, not on ns-3-allinone.

## Proposing New Modules

To recommend a new module for future inclusion in ns-allinone, please post
a pull request to https://gitlab.com/nsnam/ns-3-allinone repository to
add it to the file `MANIFEST.md`.

To test it for inclusion, you can follow these steps:

1. Run the './download.py' script, which will check out ns-3-dev and all
   modules listed in the MANIFEST.md

2. cd into ns-3-dev, and checkout the version of ns-3 that is being prepared
   for allinone release.  For instance, if ns-3.45 has been release and
   ns-allinone-3.45 is being prepared to be published shortly afterwards,
   checkout the ns-3.45 tag in ns-3-dev to test against.

3. Configure ns-3 with examples and tests, and build and run test.py.

If your module depends on additional third-party libraries (such as Boost),
your module must still compile cleanly on a system that does not have these
dependencies.  A good way to check this is to perform the above test on a
Docker container that has the minimal ns-3 requirements (CMake, Python3 and
a c++ compiler).

## History

Prior to the ns-3.45 release, ns-3-allinone was a bundle that included
the [bake packaging tool](https://gitlab.com/nsnam/bake.git), the
[NetAnim](https://gitlab.com/nsnam/netanim.git) network animator, and
ns-3.  Starting with ns-3.45, ns-3-allinone was changed to focus instead
on ns-3 and compatible contributed App Store modules.

ns-3-allinone used to have a `build.py` script, but building is now
only coordinated by the `ns3` script.
=======
# ns-allinone-3.45
ns3の最新版を新規に作成．2025/11/16~使用
>>>>>>> e1644b831fc452cffa9d620c7ea4e96272a6900e
↓使い方
CMakeビルドシステムを使ってビルドを設定します。以下のコマンドは、CMakeのPythonラッパーである を利用しています。ns3これはコマンドライン構文を簡素化し、Waf構文に似ています。ビルドを制御するオプションはいくつかありますが、通常はまず、デフォルトのビルドプロファイル（アサートが有効で、ns-3ロギングをサポート）でサンプルプログラムとテストを有効にします。

$ ./ns3 configure --enable-examples --enable-tests
次に、ns-3ns3をビルドするために使用します。

$ ./ns3 build
完了したら、ユニット テストを実行してビルドを確認できます。

$ ./test.py
すべてのテストはPASSまたはSKIPのいずれかになるはずです。これで、ns-3シミュレータが動作するようになりました。ここからプログラムを実行できます（examplesディレクトリを参照）。最初のチュートリアルプログラム（ソースコードはexamples/tutorial/first.ccにあります）を実行するには、ns3を使用してください（これにより、ns-3共有ライブラリが自動的に検出されます）。

$ ./ns3 run first
使用可能なコマンドライン オプションを表示するには、–PrintHelp引数を指定します。

$ ./ns3 run 'first --PrintHelp'

# READMEファイルを作成
cat > README.md << 'EOF'
# ns-3.45 シミュレーション環境セットアップガイド

このガイドでは、ns-3.45を使用した無線LANチャネル使用率シミュレーションの環境構築から実行までを説明します。

## 目次
1. [前提条件](#前提条件)
2. [環境構築](#環境構築)
3. [ビルド](#ビルド)
4. [実行方法](#実行方法)
5. [設定ファイル](#設定ファイル)
6. [トラブルシューティング](#トラブルシューティング)

---

## 前提条件

- macOS（Apple Silicon または Intel）
- Homebrew がインストール済み
- Xcode Command Line Tools がインストール済み

### 必要なソフトウェアのインストール
```bash
# Python 3.13をインストール（ns-3のビルドに必要）
brew install python@3.13

# yaml-cppライブラリをインストール
brew install yaml-cpp

# CMakeがインストールされていない場合
brew install cmake
```

---

## 環境構築

### 1. ディレクトリ構造

プロジェクトのディレクトリ構造は以下の通りです：
```
ns-allinone-3.45/
└── ns-3.45/
    ├── scratch/
    │   ├── scratch-simulator.cc          # 基本的なシミュレータ
    │   └── channelutilization/           # チャネル使用率シミュレーション
    │       ├── channelutilization.cc
    │       ├── config.yaml               # 設定ファイル
    │       └── CMakeLists.txt
    ├── config.yaml                        # ルートディレクトリの設定（オプション）
    └── cmake-cache/                       # ビルド成果物
```

### 2. channelutilizationディレクトリの作成
```bash
# ns-3.45ディレクトリに移動
cd /path/to/ns-allinone-3.45/ns-3.45

# channelutilizationディレクトリを作成
mkdir -p scratch/channelutilization

# channelutilization.ccをディレクトリに移動（既にscratch/にある場合）
# mv scratch/channelutilization.cc scratch/channelutilization/
```

### 3. CMakeLists.txtの作成

`scratch/channelutilization/CMakeLists.txt` を作成：
```cmake
# yaml-cppのパスを明示的に指定
set(YAML_CPP_INCLUDE_DIR "/opt/homebrew/Cellar/yaml-cpp/0.8.0/include")
set(YAML_CPP_LIBRARY "/opt/homebrew/Cellar/yaml-cpp/0.8.0/lib/libyaml-cpp.0.8.0.dylib")

# ソースファイルを取得
file(GLOB scratch_sources CONFIGURE_DEPENDS *.cc)

# 実行可能ファイルをビルド
build_exec(
  EXECNAME channelutilization
  EXECNAME_PREFIX scratch_
  SOURCE_FILES "${scratch_sources}"
  LIBRARIES_TO_LINK "${ns3-libs}" "${ns3-contrib-libs}" "${YAML_CPP_LIBRARY}"
  EXECUTABLE_DIRECTORY_PATH ${CMAKE_CURRENT_BINARY_DIR}/
)

# インクルードディレクトリを追加
target_include_directories(scratch_channelutilization PRIVATE ${YAML_CPP_INCLUDE_DIR})
```

**注意**: yaml-cppのバージョンが異なる場合は、パスを確認して修正してください：
```bash
brew list yaml-cpp  # インストールパスを確認
```

### 4. デフォルト設定ファイルの生成
```bash
# デフォルトのconfig.yamlを生成
./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default --generate-config=true

# 生成されたconfig.yamlをscratch/channelutilizationに移動
mv config.yaml scratch/channelutilization/
```

---

## ビルド

### 初回ビルド
```bash
# ns-3.45ディレクトリに移動
cd /path/to/ns-allinone-3.45/ns-3.45

# Python 3.13を使用してビルド
python3.13 ./ns3 configure --enable-examples --enable-tests
python3.13 ./ns3 build
```

### クリーンビルド（問題が発生した場合）
```bash
# キャッシュを削除
rm -rf cmake-cache

# 再ビルド
python3.13 ./ns3 configure --enable-examples --enable-tests
python3.13 ./ns3 build
```

---

## 実行方法

### 基本的な実行方法

#### 1. scratch-simulatorの実行
```bash
./cmake-cache/scratch/ns3.45-scratch-simulator-default
```

期待される出力：
```
Scratch Simulator
```

#### 2. channelutilizationの実行
```bash
# デフォルト設定で実行
./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default --config=scratch/channelutilization/config.yaml

# または、ルートディレクトリのconfig.yamlを使用
./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default --config=config.yaml
```

### 便利なエイリアスの設定（推奨）

`.zshrc`（または`.bashrc`）に以下を追加すると、どこからでも簡単に実行できます：
```bash
# .zshrcに追加
cat >> ~/.zshrc << 'EOF'

# ns-3実行用のエイリアスと関数
alias ns3-cd='cd /path/to/ns-allinone-3.45/ns-3.45'

# channelutilizationを実行する関数
run-channel() {
    cd /path/to/ns-allinone-3.45/ns-3.45
    if [ $# -eq 0 ]; then
        ./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default --config=scratch/channelutilization/config.yaml
    else
        ./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default "$@"
    fi
    cd - > /dev/null
}

# scratch-simulatorを実行する関数
run-simulator() {
    cd /path/to/ns-allinone-3.45/ns-3.45
    ./cmake-cache/scratch/ns3.45-scratch-simulator-default "$@"
    cd - > /dev/null
}

# ビルドコマンドの簡略化
ns3-build() {
    cd /path/to/ns-allinone-3.45/ns-3.45
    python3.13 ./ns3 build
    cd - > /dev/null
}
EOF

# 設定を反映
source ~/.zshrc
```

**注意**: `/path/to/ns-allinone-3.45/ns-3.45` を実際のパスに置き換えてください。

### エイリアス設定後の実行方法
```bash
# どこからでも実行可能！
run-channel                              # デフォルト設定で実行
run-channel --config=custom.yaml         # カスタム設定で実行
run-simulator                            # scratch-simulatorを実行
ns3-build                                # ビルド
ns3-cd                                   # ns-3.45ディレクトリに移動
```

---

## 設定ファイル

### config.yamlの編集
```bash
# エディタで編集
nano scratch/channelutilization/config.yaml

# または
code scratch/channelutilization/config.yaml
```

### 設定例
```yaml
# 総ユーザ数
nStations: 10

# 重ユーザ数
nHeavyUsers: 7

# 軽ユーザ数
nLightUsers: 3

# 重ユーザ割合(%) 
heavyUserPercentage: 70

# 実行環境の配置半径(m)
radius: 7.5

# 出力CSVファイル名
outputFile: "channel_utilization_results.csv"

# 重ユーザのデータレート(Mbps)
heavyUserRate: 50

# 軽ユーザのデータレート(Mbps)
lightUserRate: 20

# パケットサイズ(バイト)
packetSize: 1500

# シミュレーション時間(秒)
simulationTime: 10.0

# TXT形式の詳細結果出力を有効化
enableTxtOutput: true

# NetAnimトレース生成を無効化（パケット上限エラーを回避）
enableNetAnim: false

# 詳細ログ出力
verbose: false
```

### パラメータの説明

| パラメータ | 説明 | 推奨値 |
|-----------|------|--------|
| `nStations` | 総端末数 | 1-100 |
| `nHeavyUsers` | 重ユーザ数 | nStations以下 |
| `nLightUsers` | 軽ユーザ数 | nStations以下 |
| `heavyUserPercentage` | 重ユーザ割合(%) | (nHeavyUsers/nStations)*100 |
| `radius` | AP周りの配置半径(m) | 5.0-15.0 |
| `heavyUserRate` | 重ユーザのデータレート(Mbps) | 20-100 |
| `lightUserRate` | 軽ユーザのデータレート(Mbps) | 5-50 |
| `packetSize` | パケットサイズ(バイト) | 1024-1500 |
| `simulationTime` | シミュレーション時間(秒) | 5.0-60.0 |
| `enableNetAnim` | NetAnimトレース生成 | false推奨 |

**注意**: 
- `nStations = nHeavyUsers + nLightUsers` になるように設定してください
- `enableNetAnim: true` にすると、多数のパケットで "Max Packets per trace file exceeded" エラーが出ることがあります

---

## 結果の確認

### 出力ファイル

シミュレーション実行後、以下のファイルが生成されます：
```
ns-3.45/
├── results/
│   └── channelutilization_YYYYMMDD_HHMMSS/
│       ├── results_nXX_hYY.txt         # 詳細な統計情報
│       └── animation_nXX_hYY.xml       # NetAnimトレースファイル（enableNetAnim: trueの場合）
└── result_csv/
    └── channel_utilization_*.csv       # CSV形式の結果（累積）
```

### CSVファイルの確認
```bash
# CSV結果を表示
cat result_csv/channel_utilization_results.csv

# 最新の結果を表示
tail -1 result_csv/channel_utilization_results.csv
```

### TXT結果の確認
```bash
# 最新の結果ディレクトリを表示
ls -lt results/ | head -2

# 結果ファイルを表示
cat results/channelutilization_*/results_*.txt
```

---

## トラブルシューティング

### 1. `./ns3` スクリプトが動作しない

**エラー**: `ValueError: action 'store_true' is not valid for positional arguments`

**原因**: Python 3.14との互換性問題

**解決策**: Python 3.13を使用
```bash
brew install python@3.13
python3.13 ./ns3 build
```

### 2. yaml-cppライブラリが見つからない

**エラー**: `ld: library 'yaml-cpp' not found`

**解決策**:
```bash
# yaml-cppをインストール
brew install yaml-cpp

# パスを確認
brew list yaml-cpp

# CMakeLists.txtのパスを更新
nano scratch/channelutilization/CMakeLists.txt
```

### 3. YAMLファイルが見つからない

**エラー**: `YAML読み込みエラー: bad file: config.yaml`

**解決策**:
```bash
# 設定ファイルを生成
./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default --generate-config=true

# 正しいパスを指定
./cmake-cache/scratch/channelutilization/ns3.45-channelutilization-default --config=scratch/channelutilization/config.yaml
```

### 4. NetAnimのパケット上限エラー

**エラー**: `Max Packets per trace file exceeded`

**解決策**: `config.yaml` で NetAnim を無効化
```yaml
enableNetAnim: false
```

### 5. uart-net-deviceのビルドエラー

**エラー**: `Undefined symbols for architecture arm64`

**解決策**: 
```bash
# contrib/uart-net-deviceを無効化
cd contrib
mv uart-net-device uart-net-device.disabled

# 再ビルド
cd ..
python3.13 ./ns3 clean
python3.13 ./ns3 build
```

### 6. コンパイル警告

**警告**: `unused variable 't'`

**解決策**: これは警告であり、実行には影響しません。無視しても問題ありません。

---

## よくある質問

### Q1: どのPythonバージョンを使うべきですか？

A: Python 3.13を推奨します。Python 3.14では`./ns3`スクリプトに互換性問題があります。

### Q2: シミュレーション結果はどこに保存されますか？

A: 
- 詳細結果: `results/channelutilization_YYYYMMDD_HHMMSS/`
- CSV結果: `result_csv/`

### Q3: 複数の設定でシミュレーションを実行したい

A: 異なる名前のYAMLファイルを作成し、`--config`オプションで指定してください：
```bash
run-channel --config=scratch/channelutilization/config_scenario1.yaml
run-channel --config=scratch/channelutilization/config_scenario2.yaml
```

### Q4: NetAnimで結果を可視化したい

A: 
1. `config.yaml`で`enableNetAnim: true`に設定
2. シミュレーション実行後、生成されたXMLファイルをNetAnimで開く
3. 注意: 大量のパケットでエラーが出る場合は、`simulationTime`を短くするか、`nStations`を減らしてください

---

## 参考リンク

- [ns-3公式ドキュメント](https://www.nsnam.org/documentation/)
- [ns-3チュートリアル](https://www.nsnam.org/docs/tutorial/html/)
- [yaml-cpp GitHub](https://github.com/jbeder/yaml-cpp)

---

## ライセンス

このプロジェクトはGPL-2.0ライセンスの下で配布されています。

---

## 更新履歴

- 2025-11-18: 初版作成
  - ns-3.45環境構築手順
  - channelutilizationシミュレーション実行方法
  - トラブルシューティング追加

EOF

echo "README.mdを作成しました！"