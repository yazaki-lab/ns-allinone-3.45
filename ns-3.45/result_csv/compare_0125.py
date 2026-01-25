import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rcParams

# 日本語フォント設定
rcParams['font.sans-serif'] = ['DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

# CSVファイルの読み込み
df = pd.read_csv('cu_0119_1st.csv')

# データの抽出
load_data = df['Throughput(Mbps)'].values
rssi_data = df['AvgRSSI(dBm)'].values
stations_data = df['Stations'].values
util_measured = df['Utilization(%)'].values

# 式に基づく計算値
util_calculated = 88.64 * (1 - np.exp(-(-0.0052*load_data + -0.0062*rssi_data + 0.2169*stations_data + -0.4879)))

# Station数のユニークな値を取得
unique_stations = np.unique(stations_data)

# グラフの作成
plt.figure(figsize=(12, 8))

# 各Station数ごとにプロット
for station_num in unique_stations:
    # 該当するStation数のデータを抽出
    mask = stations_data == station_num
    rssi_filtered = rssi_data[mask]
    util_measured_filtered = util_measured[mask]
    util_calculated_filtered = util_calculated[mask]
    
    # 測定値をプロット(散布図)
    plt.scatter(rssi_filtered, util_measured_filtered, 
                label=f'Measured (Stations={int(station_num)})', 
                alpha=0.6, s=50)
    
    # 計算値を近似曲線としてプロット
    # RSSIでソートして滑らかな曲線を描画
    sorted_indices = np.argsort(rssi_filtered)
    rssi_sorted = rssi_filtered[sorted_indices]
    util_calc_sorted = util_calculated_filtered[sorted_indices]
    
    plt.plot(rssi_sorted, util_calc_sorted, 
             label=f'Calculated (Stations={int(station_num)})', 
             linewidth=2, linestyle='--')

plt.xlabel('RSSI (dBm)', fontsize=12)
plt.ylabel('Utilization (%)', fontsize=12)
plt.title('Utilization: Measured vs Calculated', fontsize=14)
plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.show()

# 統計情報の表示
print("データ統計:")
print(f"Station数の範囲: {unique_stations}")
print(f"RSSI範囲: {rssi_data.min():.2f} ~ {rssi_data.max():.2f} dBm")
print(f"測定値Utilization範囲: {util_measured.min():.2f} ~ {util_measured.max():.2f} %")
print(f"計算値Utilization範囲: {util_calculated.min():.2f} ~ {util_calculated.max():.2f} %")