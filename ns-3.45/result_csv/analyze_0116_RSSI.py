import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# === 設定 ===
# CSVファイルのパスを指定してください
csv_file_path = 'cu_0116.csv' 
#/Users/kamikawa/Desktop/ns-allinone-3.45/ns-3.45/result_csv/
# === データ読み込み ===
try:
    df = pd.read_csv(csv_file_path)
    # 必要なカラムがあるか確認
    required_cols = ['Load(Mbps)', 'AvgRSSI(dBm)', 'Utilization(%)']
    if not all(col in df.columns for col in required_cols):
        print(f"エラー: CSVファイルに以下のカラムが含まれていません: {required_cols}")
        # カラム名が微妙に違う場合の対応（スペース削除など）
        df.columns = [c.strip() for c in df.columns]
except Exception as e:
    print(f"ファイルの読み込みに失敗しました: {e}")
    # ダミーデータでの実行例を示すための処理（実際には不要）
    df = pd.DataFrame()

# === 近似モデルの定義 ===
# モデル: シグモイド関数ベースの容量モデル + 負荷係数
# RSSIが良い(大きい)ほど分母が大きくなり、Utilizationは下がる(同じLoadなら短時間で送れるため)
# 逆にRSSIが悪いと分母が小さくなり、Utilizationは上がる。上限は100(または90程度)。
def model_func(x, a, b, c, d):
    # x: RSSI
    # a: Loadに依存する係数（スケーリング）
    # b: 曲線の急峻さ
    # c: 変曲点（RSSI）
    # d: ベースライン
    val = (a) / (1 + np.exp(-b * (x - c))) + d
    return np.minimum(val, 95.0) # 95%でサチュレーションさせる

# === Loadごとに分析 ===
loads = df['Load(Mbps)'].unique()
loads.sort()

plt.figure(figsize=(10, 6))

colors = plt.cm.viridis(np.linspace(0, 1, len(loads)))

print("--- 近似式のパラメータ算出結果 ---")

for i, load in enumerate(loads):
    subset = df[df['Load(Mbps)'] == load]
    
    # データ数が少なすぎる場合はスキップ
    if len(subset) < 3:
        continue
        
    rssi = subset['AvgRSSI(dBm)'].values
    util = subset['Utilization(%)'].values
    
    # プロット
    plt.scatter(rssi, util, label=f'Load: {load} Mbps', color=colors[i], alpha=0.7)
    
    # フィッティング（Load=100Mbpsなど、飽和していないデータで特に有効）
    # パラメータの初期推定値 [a, b, c, d]
    p0 = [100, 0.1, -70, 0] 
    try:
        # RSSIとUtilの関係をフィッティング
        # 注意: Loadが固定なので、Utilizationは RSSI の関数 U = f(RSSI) になる
        # ここでは「RSSIが低い -> Utilが高い」「RSSIが高い -> Utilが低い」という逆S字を想定
        # 式: U = A / (1 + exp(B*(RSSI - C))) + D 
        # Bが正なら減少関数になる(分母が増えるため)
        
        def fit_func_per_load(r, A, B, C, D):
            return A / (1 + np.exp(B * (r - C))) + D
            
        popt, _ = curve_fit(fit_func_per_load, rssi, util, p0=p0, maxfev=10000)
        
        # 近似曲線の描画
        x_range = np.linspace(min(rssi)-5, max(rssi)+5, 100)
        y_fit = fit_func_per_load(x_range, *popt)
        y_fit = np.minimum(y_fit, 100) # グラフ上の見た目のクリップ
        
        plt.plot(x_range, y_fit, linestyle='--', color=colors[i])
        
        print(f"Load {load} Mbps: Util ≈ {popt[0]:.2f} / (1 + exp({popt[1]:.2f} * (RSSI - {popt[2]:.2f}))) + {popt[3]:.2f}")
        
    except Exception as e:
        print(f"Load {load} Mbps: フィッティング失敗 ({e})")

plt.xlabel('Avg RSSI (dBm)')
plt.ylabel('Channel Utilization (%)')
plt.title('RSSI vs Channel Utilization per Load')
plt.legend()
plt.grid(True)
plt.show()