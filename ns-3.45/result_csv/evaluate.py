#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
evaluate.py
- Read one or more CSVs
- Build dataset: U (target), RSSI, N, L_total, L_per, distance
- Evaluate baselines:
    RSSI only
    N only
    (RSSI, N)
    (RSSI, N, L)  where L can be total or per-station
- Models: Linear / DecisionTree / RandomForest
- Metrics: MAE / RMSE / R2
- Also split metrics for N<threshold and N>=threshold
- CV strategy: leave-one-distance-out (GroupKFold) if distance exists
"""

import argparse
import os
from dataclasses import dataclass
from typing import List, Optional, Tuple, Dict

import numpy as np
import pandas as pd

from sklearn.model_selection import GroupKFold, GroupShuffleSplit
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LinearRegression
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score


# ----------------------------
# Column detection
# ----------------------------
def find_col(df: pd.DataFrame, candidates: List[str]) -> Optional[str]:
    for c in candidates:
        if c in df.columns:
            return c
    # case-insensitive fallback
    lower_map = {c.lower(): c for c in df.columns}
    for c in candidates:
        if c.lower() in lower_map:
            return lower_map[c.lower()]
    return None


@dataclass
class ColMap:
    U: str
    RSSI: str
    N: str
    L: Optional[str]
    distance: Optional[str]


def detect_columns(df: pd.DataFrame) -> ColMap:
    # Your known columns (plus some common variants)
    u_col = find_col(df, ["U", "Utilization(%)", "Utilization", "channel_util", "ChannelUtil(%)"])
    rssi_col = find_col(df, ["RSSI", "AvgRSSI(dBm)", "AvgRSSI", "rssi_dbm"])
    n_col = find_col(df, ["N", "Stations", "Clients", "num_stations"])
    l_col = find_col(df, ["L", "Load(Mbps)", "Load", "offered_load_mbps"])
    d_col = find_col(df, ["distance", "Radius(m)", "Radius", "Dist(m)", "r_m"])

    missing = []
    if u_col is None: missing.append("U / Utilization(%)")
    if rssi_col is None: missing.append("RSSI / AvgRSSI(dBm)")
    if n_col is None: missing.append("N / Stations")

    if missing:
        raise ValueError(f"Missing required columns: {missing}\nColumns in file: {list(df.columns)}")

    return ColMap(U=u_col, RSSI=rssi_col, N=n_col, L=l_col, distance=d_col)


# ----------------------------
# Metrics
# ----------------------------
def metrics(y_true: np.ndarray, y_pred: np.ndarray) -> Tuple[float, float, float]:
    mae = mean_absolute_error(y_true, y_pred)
    mse = mean_squared_error(y_true, y_pred)
    rmse = float(np.sqrt(mse))
    r2 = r2_score(y_true, y_pred)
    return mae, rmse, r2


def safe_metrics(y_true: np.ndarray, y_pred: np.ndarray) -> Tuple[float, float, float]:
    if len(y_true) == 0:
        return (np.nan, np.nan, np.nan)
    return metrics(y_true, y_pred)


# ----------------------------
# Build dataset
# ----------------------------
def load_and_build(csv_paths: List[str]) -> pd.DataFrame:
    frames = []
    for p in csv_paths:
        df = pd.read_csv(p)
        cm = detect_columns(df)

        out = pd.DataFrame()
        out["U"] = df[cm.U].astype(float)
        out["RSSI"] = df[cm.RSSI].astype(float)
        out["N"] = df[cm.N].astype(float)

        if cm.L is not None:
            out["L_total"] = df[cm.L].astype(float)
            # L is total load (= per-station * N) in your data
            out["L_per"] = out["L_total"] / out["N"].replace(0, np.nan)
        else:
            out["L_total"] = np.nan
            out["L_per"] = np.nan

        if cm.distance is not None:
            out["distance"] = df[cm.distance].astype(float)
        else:
            out["distance"] = np.nan

        out["source"] = os.path.basename(p)
        frames.append(out)

    all_df = pd.concat(frames, ignore_index=True)

    # Clean rows
    all_df = all_df.replace([np.inf, -np.inf], np.nan)
    all_df = all_df.dropna(subset=["U", "RSSI", "N"]).reset_index(drop=True)

    return all_df


# ----------------------------
# Evaluation
# ----------------------------
def evaluate(
    df: pd.DataFrame,
    outdir: str,
    l_feature: str,
    threshold: int,
    seed: int,
    n_estimators: int,
    min_samples_leaf: int,
) -> pd.DataFrame:
    os.makedirs(outdir, exist_ok=True)

    # Choose which L to use in (RSSI,N,L)
    if l_feature == "total":
        L_col = "L_total"
    elif l_feature == "per_station":
        L_col = "L_per"
    else:
        raise ValueError("--l_feature must be 'total' or 'per_station'")

    has_L = df[L_col].notna().any()

    # Feature sets (exactly what you wanted)
    feature_sets: List[Tuple[str, List[str]]] = [
            ("RSSI_only", ["RSSI"]),
            ("N_only", ["N"]),
            (f"{L_col}_only", [L_col]),

            ("RSSI_N", ["RSSI","N"]),
            (f"RSSI_{L_col}", ["RSSI", L_col]),
            (f"N_{L_col}", ["N", L_col]),
    ]
    if has_L:
        feature_sets.append((f"RSSI_N_L({L_col})", ["RSSI", "N", L_col]))

    # Models
    models = [
        ("Linear", Pipeline([("scaler", StandardScaler()), ("model", LinearRegression())])),
        ("Tree", DecisionTreeRegressor(random_state=seed)),
        ("RF", RandomForestRegressor(
            random_state=seed, #種を固定して再現性確保
            n_estimators=n_estimators, #木の数（多いほど精度向上・過学習の可能性も増える・計算コスト増）
            min_samples_leaf=min_samples_leaf, #葉の数が少ないと過学習の可能性がある
            n_jobs = -1 #CPUコア全使用
        )),
    ]

    # Group CV: leave-one-distance-out if possible
    use_group = df["distance"].notna().any()
    if use_group:
        groups = df["distance"].to_numpy()
        unique_groups = np.unique(groups[~np.isnan(groups)])
        n_splits = len(unique_groups)
        if n_splits < 2:
            use_group = False

    rows: List[Dict] = []
    y = df["U"].to_numpy()

    # Helper: run one fold or one split
    def run_fit_predict(model, X_train, y_train, X_test):
        model.fit(X_train, y_train)
        return model.predict(X_test)

    if use_group:
        gkf = GroupKFold(n_splits=n_splits)
        splits = list(gkf.split(df, y, groups=groups))  # ★ここが重要
        split_name = "GroupKFold(leave-one-distance-out)"
    else:
        splitter = GroupShuffleSplit(n_splits=1, test_size=0.2, random_state=seed)
        splits = list(splitter.split(df, y, groups=None))  # ★list化
        split_name = "GroupShuffleSplit(random 80/20) [distance missing]"

    # Evaluate
    for fs_name, cols in feature_sets:
        X = df[cols].to_numpy()

        for model_name, model in models:
            fold_all = []
            fold_low = []
            fold_high = []

            # (optional) store OOF predictions for plotting best model later
            oof_pred = np.full(len(df), np.nan, dtype=float)

            for train_idx, test_idx in splits:
                X_train, X_test = X[train_idx], X[test_idx]
                y_train, y_test = y[train_idx], y[test_idx]
                pred = run_fit_predict(model, X_train, y_train, X_test)

                oof_pred[test_idx] = pred

                # all
                fold_all.append(metrics(y_test, pred))

                # by N threshold
                N_test = df.iloc[test_idx]["N"].to_numpy()
                mask_low = N_test < threshold
                mask_high = N_test >= threshold

                fold_low.append(safe_metrics(y_test[mask_low], pred[mask_low]))
                fold_high.append(safe_metrics(y_test[mask_high], pred[mask_high]))

            def summarize(name: str, vals: List[Tuple[float, float, float]]):
                arr = np.array(vals, dtype=float)
                out = {
                    "split": split_name,
                    "features": fs_name,
                    "model": model_name,
                    "subset": name,
                    "MAE_mean": float(np.nanmean(arr[:, 0])),
                    "MAE_std": float(np.nanstd(arr[:, 0], ddof=1)) if np.sum(~np.isnan(arr[:, 0])) > 1 else 0.0,
                    "RMSE_mean": float(np.nanmean(arr[:, 1])),
                    "RMSE_std": float(np.nanstd(arr[:, 1], ddof=1)) if np.sum(~np.isnan(arr[:, 1])) > 1 else 0.0,
                    "R2_mean": float(np.nanmean(arr[:, 2])),
                    "R2_std": float(np.nanstd(arr[:, 2], ddof=1)) if np.sum(~np.isnan(arr[:, 2])) > 1 else 0.0,
                    "folds_used": int(np.sum(~np.isnan(arr[:, 1]))),
                }
                return out

            rows.append(summarize("all", fold_all))
            rows.append(summarize(f"N<{threshold}", fold_low))
            rows.append(summarize(f"N>={threshold}", fold_high))

            # Save OOF predictions for this model/features (useful for plots later)
            # Only for RF + RSSI_N_L(...) by default
            if fs_name.startswith("RSSI_N_L") and model_name == "RF":
                oof_df = df.copy()
                oof_df["pred_U"] = oof_pred
                oof_df["residual"] = oof_df["pred_U"] - oof_df["U"]
                oof_path = os.path.join(outdir, f"oof_predictions_{fs_name}_{model_name}.csv".replace("/", "_"))
                oof_df.to_csv(oof_path, index=False)

        # # reset split iterator if it was a generator that got consumed
        # if use_group:
        #     split_iter = gkf.split(df, y, groups=df["distance"].to_numpy())
        # else:
        #     split_iter = splitter.split(df, y, groups=None)

    res = pd.DataFrame(rows)
    res = res.sort_values(["subset", "RMSE_mean"], ascending=[True, True]).reset_index(drop=True)

    res_path = os.path.join(outdir, "results_cv.csv")
    res.to_csv(res_path, index=False)

    # Slide-friendly: RF only
    rf = res[res["model"] == "RF"].copy()
    rf_path = os.path.join(outdir, "results_RF_only.csv")
    rf.to_csv(rf_path, index=False)

    # Print summary to console
    with pd.option_context("display.max_rows", 200, "display.max_columns", 50, "display.width", 180):
        print("\n=== CV Results (sorted by subset, RMSE_mean) ===")
        print(res)

    print(f"\nSaved:\n- {res_path}\n- {rf_path}")
    return res


# ----------------------------
# Main
# ----------------------------
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--csv", nargs="+", required=True, help="Input CSV paths (one or more)")
    ap.add_argument("--outdir", default="out", help="Output directory")
    ap.add_argument("--l_feature", default="total", choices=["total", "per_station"],
                    help="Use L_total (default) or L_per (=L_total/N) for (RSSI,N,L)")
    ap.add_argument("--threshold", type=int, default=9, help="Threshold for N split (default: 9)")
    ap.add_argument("--seed", type=int, default=42)
    ap.add_argument("--n_estimators", type=int, default=500, help="RF trees")
    ap.add_argument("--min_samples_leaf", type=int, default=2, help="RF min_samples_leaf")
    args = ap.parse_args()

    df = load_and_build(args.csv)

    # Save combined dataset for reference
    os.makedirs(args.outdir, exist_ok=True)
    dataset_path = os.path.join(args.outdir, "dataset_combined.csv")
    df.to_csv(dataset_path, index=False)
    print(f"Saved combined dataset: {dataset_path}")

    # Quick sanity print
    print("\nDataset head:")
    print(df.head())
    print("\nCounts by distance:")
    if df["distance"].notna().any():
        print(df.groupby("distance").size())
    else:
        print("distance column not found -> random 80/20 split will be used")

    evaluate(
        df=df,
        outdir=args.outdir,
        l_feature=args.l_feature,
        threshold=args.threshold,
        seed=args.seed,
        n_estimators=args.n_estimators,
        min_samples_leaf=args.min_samples_leaf,
    )


if __name__ == "__main__":
    main()