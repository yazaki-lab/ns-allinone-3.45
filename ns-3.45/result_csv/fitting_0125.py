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
    z = np.clip(z, -50, 50)

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

# =========================================================
# 5. 測定値のみの3次元散布図
# =========================================================
fig = plt.figure(figsize=(8, 6))
ax = fig.add_subplot(111, projection='3d')

# 測定データ
x = df['Load(Mbps)'].values
y = df['AvgRSSI(dBm)'].values
z = df['Utilization(%)'].values
c = df['Stations'].values  # 色に使用

# 3D scatter
sc = ax.scatter(
    x, y, z,
    c=c,
    cmap='viridis',
    s=40,
    alpha=0.8
)

# 軸ラベル
ax.set_xlabel('Load (Mbps)')
ax.set_ylabel('Avg RSSI (dBm)')
ax.set_zlabel('Utilization (%)')

# カラーバー（Stations）
cb = plt.colorbar(sc, ax=ax, pad=0.1)
cb.set_label('Stations')

ax.set_title('Measured Data Only (3D Scatter)')

plt.show()
