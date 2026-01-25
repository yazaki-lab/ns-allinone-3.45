# =========================================================
# 必要なライブラリの読み込み
# =========================================================
# pandas        : CSVファイルを読み込み、表形式のデータを扱う
# numpy         : 数値計算（配列・指数関数など）
# curve_fit     : 非線形回帰（今回はシグモイド関数）の係数推定
# r2_score      : モデルの当てはまりの良さ（決定係数R²）を計算
# matplotlib    : グラフ描画用
# =========================================================
import pandas as pd
import numpy as np
from scipy.optimize import curve_fit
from sklearn.metrics import r2_score
import matplotlib.pyplot as plt


# =========================================================
# 1. データの読み込み
# =========================================================
# ns-3のシミュレーション結果をCSVファイルから読み込む
# ここでは「チャネル使用率(Utilization)」を説明するための
# 入力データ（Load, RSSI, Stations）が含まれている想定
# =========================================================
file_path = 'cu_0119_1st.csv'

try:
    # CSVファイルをDataFrameとして読み込む
    df = pd.read_csv(file_path)
    print(f"読み込み成功: {len(df)} 行のデータを取得しました。")
except FileNotFoundError:
    # ファイルが見つからなかった場合のエラー処理
    print(f"エラー: ファイルが見つかりません。\nパスを確認してください: {file_path}")
    exit()


# =========================================================
# 2'. モデル関数の定義（指数飽和モデル）
# =========================================================
def exp_saturation_model(X, a, b, c, d, max_val):
    """
    指数飽和型回帰モデル

    Utilization = max_val * (1 - exp(-(a*Load + b*RSSI + c*Stations + d)))
    """

    load, rssi, stations = X

    z = a * load + b * rssi + c * stations + d

    # 安全対策（expの発散防止）
    z = np.clip(z, -30, 30)

    return max_val * (1 - np.exp(-z))


# =========================================================
# 3. 学習データの準備
# =========================================================
# 説明変数（入力）
#   Load(Mbps)      : トラフィック量
#   AvgRSSI(dBm)    : 平均受信電力
#   Stations        : 接続端末数
#
# 目的変数（出力）
#   Utilization(%)  : チャネル使用率
# =========================================================
X_data = (
    df['Load(Mbps)'].values,
    df['AvgRSSI(dBm)'].values,
    df['Stations'].values
)

y_data = df['Utilization(%)'].values


# =========================================================
# 4. 非線形回帰（フィッティング）の実行
# =========================================================
# p0 はパラメータの初期値
# ・Load は増えるほど使用率が上がる → 正
# ・RSSI は良いほど効率が良い → 負を想定
# ・Stations は増えるとオーバーヘッド増加 → 正
# =========================================================
initial_guess = [
    0.02,   # a: Load（急峻なのでシグモイドより小さめ）
   -0.02,   # b: RSSI（良いRSSIほど効率↑ → 負）
    0.01,   # c: Stations（競合増）
    0.0,    # d: バイアス
    95.0    # max_val: 飽和値
]


try:
    # curve_fit により最適な係数を推定
    popt, pcov = curve_fit(
        exp_saturation_model,
        X_data,
        y_data,
        p0=initial_guess,
        maxfev=20000
    )

    # 推定されたモデルで予測値を計算
    y_pred = exp_saturation_model(X_data, *popt)

    # 決定係数 R² を計算（1に近いほど良い）
    r2 = r2_score(y_data, y_pred)

    # =====================================================
    # 結果の表示
    # =====================================================
    print("\n=== 指数飽和モデルの結果 ===")
    print(f"決定係数 (R2): {r2:.4f}")
    print("-" * 60)

    print(
        f"Utilization = {popt[4]:.2f} * "
        f"(1 - exp(-({popt[0]:.4f}*Load "
        f"+ {popt[1]:.4f}*RSSI "
        f"+ {popt[2]:.4f}*Stations "
        f"+ {popt[3]:.4f})))"
    )

    print("-" * 60)
    print("【係数の解釈】")
    print(f"  a (Load)     : {popt[0]:.4f}")
    print(f"  b (RSSI)     : {popt[1]:.4f}")
    print(f"  c (Stations) : {popt[2]:.4f}")
    print(f"  d (bias)     : {popt[3]:.4f}")
    print(f"  MaxVal       : {popt[4]:.2f} %")

    # =====================================================
    # Excel 用の数式出力
    # =====================================================
    print("\n=== Excel貼り付け用 数式 ===")
    print("【前提】")
    print("  C2 = Load (Mbps)")
    print("  S2 = Avg RSSI (dBm)")
    print("  A2 = Stations")
    print("  出力セルにそのまま貼り付け可能")
    print("-" * 60)

    excel_formula = (
        f"={popt[4]:.6f}*(1-EXP(-("
        f"{popt[0]:.6f}*C2"
        f"{popt[1]:+.6f}*S2"
        f"{popt[2]:+.6f}*A2"
        f"{popt[3]:+.6f}"
        f")))"
    )

    print("Excel数式:")
    print(excel_formula)
    print("-" * 60)


    # =====================================================
    # 実測値 vs 予測値の可視化
    # =====================================================
    plt.figure(figsize=(6, 6))

    # 散布図：各点が1サンプル
    plt.scatter(y_data, y_pred, alpha=0.6)

    # 理想線（完全一致：y = x）
    plt.plot([0, 100], [0, 100], 'r--', label='Perfect Fit')

    plt.xlabel('Measured Utilization (%)')
    plt.ylabel('Predicted Utilization (%)')
    plt.title(f'Measured vs Predicted (R2={r2:.4f})')
    plt.legend()
    plt.grid(True)
    plt.show()

except Exception as e:
    # フィッティングに失敗した場合のエラー表示
    print(f"フィッティングエラーが発生しました: {e}")
    popt = None

# =========================================================
# 5. 測定値 + 予測曲面の3次元散布図
# =========================================================
if popt is not None:
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')

    # 測定データ
    x_measured = df['Load(Mbps)'].values
    y_measured = df['AvgRSSI(dBm)'].values
    z_measured = df['Utilization(%)'].values
    c_measured = df['Stations'].values  # 色に使用

    # 測定データの3D scatter
    sc = ax.scatter(
        x_measured, y_measured, z_measured,
        c=c_measured,
        cmap='viridis',
        s=40,
        alpha=0.8,
        label='Measured Data'
    )

    # =====================================================
    # 予測曲面の生成（複数のStations代表値）
    # =====================================================
    # Load と RSSI のメッシュグリッドを作成
    load_range = np.linspace(x_measured.min(), x_measured.max(), 30)
    rssi_range = np.linspace(y_measured.min(), y_measured.max(), 30)
    load_grid, rssi_grid = np.meshgrid(load_range, rssi_range)

    # Stationsの代表値を複数設定（最小値、25%点、中央値、75%点、最大値）
    stations_min = c_measured.min()
    stations_25 = np.percentile(c_measured, 25)
    stations_median = np.median(c_measured)
    stations_75 = np.percentile(c_measured, 75)
    stations_max = c_measured.max()
    
    representative_stations = [stations_min, stations_25, stations_median, stations_75, stations_max]
    colors = ['blue', 'cyan', 'red', 'orange', 'darkred']
    
    # 各代表値について予測曲面を描画
    for stations_val, color in zip(representative_stations, colors):
        # 予測曲面のUtilizationを計算
        utilization_grid = exp_saturation_model(
            (load_grid, rssi_grid, np.full_like(load_grid, stations_val)),
            *popt
        )

        # 予測曲面をプロット
        ax.plot_surface(
            load_grid, rssi_grid, utilization_grid,
            color=color,
            alpha=0.25,
            label=f'Stations={stations_val:.0f}'
        )

    # 軸ラベル
    ax.set_xlabel('Load (Mbps)')
    ax.set_ylabel('Avg RSSI (dBm)')
    ax.set_zlabel('Utilization (%)')

    # カラーバー（Stations）
    cb = plt.colorbar(sc, ax=ax, pad=0.1)
    cb.set_label('Stations')

    ax.set_title(f'Measured Data + Predicted Surfaces (Multiple Station Values)')

    # 凡例を手動で追加
    from matplotlib.patches import Patch
    legend_elements = [
        Patch(facecolor='blue', alpha=0.25, label=f'Stations={stations_min:.0f}'),
        Patch(facecolor='cyan', alpha=0.25, label=f'Stations={stations_25:.0f}'),
        Patch(facecolor='red', alpha=0.25, label=f'Stations={stations_median:.0f}'),
        Patch(facecolor='orange', alpha=0.25, label=f'Stations={stations_75:.0f}'),
        Patch(facecolor='darkred', alpha=0.25, label=f'Stations={stations_max:.0f}')
    ]
    ax.legend(handles=legend_elements, loc='upper left')

    plt.show()


# =========================================================
# 6. Stationsの代表点ごとの Utilization vs RSSI グラフ（2次元）
# =========================================================
if popt is not None:
    # Stationsの代表値（既に定義済み）
    representative_stations = [stations_min, stations_25, stations_median, stations_75, stations_max]
    colors = ['blue', 'cyan', 'green', 'orange', 'red']
    
    # =========================================================
    # 各Stations代表値を1つのグラフにまとめた版
    # =========================================================
    fig, ax = plt.subplots(figsize=(12, 8))
    
    for idx, (stations_val, color) in enumerate(zip(representative_stations, colors)):
        # 該当するStations値に近いデータを抽出
        tolerance = 2  # Stations値の許容範囲
        mask = np.abs(c_measured - stations_val) <= tolerance
        
        if np.sum(mask) > 0:
            # 測定データ
            rssi_measured_subset = y_measured[mask]
            util_measured_subset = z_measured[mask]
            
            # 測定データをプロット（実測値）
            ax.scatter(
                rssi_measured_subset,
                util_measured_subset,
                c=color,
                s=60,
                alpha=0.6,
                edgecolors='black',
                linewidth=0.5,
                label=f'Measured (Stations={stations_val:.0f})'
            )
            
            # 予測曲線の生成（中央値のLoadを使用）
            rssi_range = np.linspace(y_measured.min(), y_measured.max(), 100)
            load_median = np.median(x_measured)
            
            util_pred = exp_saturation_model(
                (np.full_like(rssi_range, load_median),
                 rssi_range,
                 np.full_like(rssi_range, stations_val)),
                *popt
            )
            
            # 予測曲線をプロット
            ax.plot(
                rssi_range,
                util_pred,
                color=color,
                linewidth=2.5,
                linestyle='--',
                alpha=0.9,
                label=f'Predicted (Stations={stations_val:.0f})'
            )
    
    ax.set_xlabel('RSSI (dBm)', fontsize=12)
    ax.set_ylabel('Utilization (%)', fontsize=12)
    ax.set_title(f'Utilization vs RSSI for Different Station Values (Load={load_median:.1f} Mbps)', 
                 fontsize=14, fontweight='bold')
    ax.grid(True, alpha=0.3)
    ax.legend(fontsize=10, loc='best', ncol=2)
    
    plt.tight_layout()
    plt.show()