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
file_path = '/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/result_csv/cu_0119_1st.csv'

try:
    # CSVファイルをDataFrameとして読み込む
    df = pd.read_csv(file_path)
    print(f"読み込み成功: {len(df)} 行のデータを取得しました。")
except FileNotFoundError:
    # ファイルが見つからなかった場合のエラー処理
    print(f"エラー: ファイルが見つかりません。\nパスを確認してください: {file_path}")
    exit()


# =========================================================
# 2. モデル関数の定義（シグモイド関数）
# =========================================================
# curve_fit に渡す「予測モデル」を定義する
#
# この関数は以下の考え方に基づく：
# ・Load（トラフィック量）
# ・RSSI（電波強度）
# ・Stations（接続端末数）
#
# これらの線形結合を z とし，
# それをシグモイド関数に通して
# 「チャネル使用率（0〜MaxValで飽和）」を表現する
#
# Utilization = MaxVal / (1 + exp(-z))
# =========================================================
def sigmoid_model(X, a, b, c, d, max_val):
    """
    シグモイド型の回帰モデル

    Parameters
    ----------
    X : tuple
        (Load, RSSI, Stations) の3つの配列をまとめたもの
    a : float
        Load に対する係数
    b : float
        RSSI に対する係数
    c : float
        Stations に対する係数
    d : float
        定数項（バイアス）
    max_val : float
        使用率の最大値（理論的な上限）

    Returns
    -------
    utilization : ndarray
        予測されたチャネル使用率（%）
    """

    # X に含まれる各説明変数を取り出す
    load, rssi, stations = X

    # 線形結合（回帰式の中身）
    z = a * load + b * rssi + c * stations + d

    # シグモイド関数に通して出力
    return max_val / (1 + np.exp(-z))


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
initial_guess = [0.1, -0.1, 0.01, 0, 90]

try:
    # curve_fit により最適な係数を推定
    popt, pcov = curve_fit(
        sigmoid_model,
        X_data,
        y_data,
        p0=initial_guess,
        maxfev=10000
    )

    # 推定されたモデルで予測値を計算
    y_pred = sigmoid_model(X_data, *popt)

    # 決定係数 R² を計算（1に近いほど良い）
    r2 = r2_score(y_data, y_pred)

    # =====================================================
    # 結果の表示
    # =====================================================
    print("\n=== 非線形回帰モデル（シグモイド）の結果 ===")
    print(f"決定係数 (R2): {r2:.4f}")
    print("-" * 60)

    # 論文・レポート用にそのまま書ける数式形式
    print(
        f"導出された式:\n"
        f"Utilization = {popt[4]:.2f} / "
        f"(1 + exp(-({popt[0]:.4f}*Load "
        f"+ {popt[1]:.4f}*RSSI "
        f"+ {popt[2]:.4f}*Stations "
        f"+ {popt[3]:.4f})))"
    )
    print("-" * 60)

    # 各係数の意味を説明
    print("【係数の解釈】")
    print(f"  a (Loadの影響)     : {popt[0]:.4f}")
    print(f"  b (RSSIの影響)     : {popt[1]:.4f}")
    print(f"  c (Stationsの影響) : {popt[2]:.4f}")
    print(f"  d (定数項)         : {popt[3]:.4f}")
    print(f"  MaxVal (飽和値)    : {popt[4]:.2f} %")

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
