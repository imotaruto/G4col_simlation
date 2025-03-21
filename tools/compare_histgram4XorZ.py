import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys
import matplotlib as mpl

# グローバルなフォント設定をリセット（必要に応じて）
mpl.rcParams.update(mpl.rcParamsDefault)

def load_data_no_aggregate(file_path):
    """
    入力ファイルを読み込み、X_bin, Z_bin, Count をそのまま保持した DataFrame を返す。
    さらに、X_bin 用に Count を 300 で、Z_bin 用に Count を 592 で割った列を追加。
    """
    df = pd.read_csv(file_path, delim_whitespace=True)
    df["x_scaled"] = df["Count"] / 300
    df["z_scaled"] = df["Count"] / 592
    return df

def plot_x_range(df1, df2, start_bin, end_bin, output_filename, title=""):
    """
    2つの DataFrame から、X_bin が [start_bin, end_bin] にある行のみ取り出し、
    各ピクセルごとに File1 / File2 の x_scaled を棒グラフで比較する。
    縦軸はログスケール。
    """
    # ファイル1とファイル2を指定範囲でフィルタリング
    sub1 = df1[(df1["X_bin"] >= start_bin) & (df1["X_bin"] <= end_bin)].copy()
    sub2 = df2[(df2["X_bin"] >= start_bin) & (df2["X_bin"] <= end_bin)].copy()
    
    # 同じX_binに複数のエントリがある場合、合算して一意にする
    sub1 = sub1.groupby("X_bin")["x_scaled"].sum().reset_index()
    sub2 = sub2.groupby("X_bin")["x_scaled"].sum().reset_index()
    
    # X_bin をインデックスに設定し、ソート
    sub1 = sub1.set_index("X_bin").sort_index()
    sub2 = sub2.set_index("X_bin").sort_index()
    
    # 両方の DataFrame に存在する全てのX_binを取得
    all_bins = sorted(set(sub1.index).union(sub2.index))
    
    # reindex していない bin には 0 を埋める
    sub1 = sub1.reindex(all_bins, fill_value=0)
    sub2 = sub2.reindex(all_bins, fill_value=0)
    
    # グループ化棒グラフ用のデータ準備
    x_positions = np.arange(len(all_bins))
    bar_width = 0.35  # 棒グラフの幅
    
    fig, ax = plt.subplots(figsize=(14, 7))
    
    # File1 のバー
    ax.bar(
        x_positions - bar_width/2,
        sub1["x_scaled"],
        width=bar_width,
        label="Cu",
        alpha=0.7,
        color='blue'
    )
    # File2 のバー
    ax.bar(
        x_positions + bar_width/2,
        sub2["x_scaled"],
        width=bar_width,
        label="W",
        alpha=0.7,
        color='orange'
    )
    
    # ログスケール
    ax.set_yscale('log')
    
    # 軸ラベルやタイトルなど
    ax.set_xlabel(f"X-Pixel", fontsize=25)
    ax.set_ylabel("Counts/Pixel", fontsize=25)
    ax.set_title(title, fontsize=30)
    
    # X 軸の目盛りを bin に合わせる
    ax.set_xticks(x_positions)
    ax.set_xticklabels(all_bins, fontsize=20, rotation=90)
    
    # Y 軸の目盛り設定
    ax.tick_params(axis='y', labelsize=20)
    
    # 凡例の設定
    ax.legend(fontsize=20)
    
    # グリッドの追加
    ax.grid(True, which="both", linestyle='--', linewidth=0.5, color='gray')
    
    fig.tight_layout()
    fig.savefig(output_filename)
    plt.close(fig)

def plot_z_range(df1, df2, start_bin, end_bin, output_filename, title=""):
    """
    2つの DataFrame から、Z_bin が [start_bin, end_bin] にある行のみ取り出し、
    各ピクセルごとに File1 / File2 の z_scaled を棒グラフで比較する。
    縦軸はログスケール。
    """
    # ファイル1とファイル2を指定範囲でフィルタリング
    sub1 = df1[(df1["Z_bin"] >= start_bin) & (df1["Z_bin"] <= end_bin)].copy()
    sub2 = df2[(df2["Z_bin"] >= start_bin) & (df2["Z_bin"] <= end_bin)].copy()
    
    # 同じZ_binに複数のエントリがある場合、合算して一意にする
    sub1 = sub1.groupby("Z_bin")["z_scaled"].sum().reset_index()
    sub2 = sub2.groupby("Z_bin")["z_scaled"].sum().reset_index()
    
    # Z_bin をインデックスに設定し、ソート
    sub1 = sub1.set_index("Z_bin").sort_index()
    sub2 = sub2.set_index("Z_bin").sort_index()
    
    # 両方の DataFrame に存在する全てのZ_binを取得
    all_bins = sorted(set(sub1.index).union(sub2.index))
    
    # reindex していない bin には 0 を埋める
    sub1 = sub1.reindex(all_bins, fill_value=0)
    sub2 = sub2.reindex(all_bins, fill_value=0)
    
    # グループ化棒グラフ用のデータ準備
    x_positions = np.arange(len(all_bins))
    bar_width = 0.35  # 棒グラフの幅
    
    fig, ax = plt.subplots(figsize=(14, 7))
    
    # File1 のバー
    ax.bar(
        x_positions - bar_width/2,
        sub1["z_scaled"],
        width=bar_width,
        label="Cu",
        alpha=0.7,
        color='blue'
    )
    # File2 のバー
    ax.bar(
        x_positions + bar_width/2,
        sub2["z_scaled"],
        width=bar_width,
        label="W",
        alpha=0.7,
        color='orange'
    )
    
    # ログスケール
    ax.set_yscale('log')
    
    # 軸ラベルやタイトルなど
    ax.set_xlabel(f"Z-Pixel", fontsize=25)
    ax.set_ylabel("Counts/Pixel", fontsize=25)
    ax.set_title(title, fontsize=30)
    
    # X 軸の目盛りを bin に合わせる
    ax.set_xticks(x_positions)
    ax.set_xticklabels(all_bins, fontsize=20, rotation=90)
    
    # Y 軸の目盛り設定
    ax.tick_params(axis='y', labelsize=20)
    
    # 凡例の設定
    ax.legend(fontsize=20)
    
    # グリッドの追加
    ax.grid(True, which="both", linestyle='--', linewidth=0.5, color='gray')
    
    fig.tight_layout()
    fig.savefig(output_filename)
    plt.close(fig)

if __name__ == "__main__":
    """
    使い方：
        python compare_histgram4XorZ.py <input_file1> <input_file2>

    このスクリプトは以下を実行します：
      1. <input_file1> と <input_file2> からデータを読み込み、
         X_bin, Z_bin ごとに生の Count を保持（合算は X_bin または Z_bin 内で）。
         同時に X方向の値は Count/300, Z方向の値は Count/592 にスケーリングした列を作る。
      2. X_bin:  (150–170) と (220–240)
         Z_bin:  (0–20)     と (260–280)
         それぞれ４つの範囲ごとに１ピクセルずつ棒グラフを作成し、
         同じ bin 上に File1 と File2 を並べて比較する。
      3. 出力されるグラフは:
         - comparison_X_150_170.png
         - comparison_X_220_240.png
         - comparison_Z_0_20.png
         - comparison_Z_260_280.png
         の4つ。
    """
    if len(sys.argv) != 3:
        print("Usage: python compare_histgram4XorZ.py <input_file1> <input_file2>")
        sys.exit(1)

    file1 = sys.argv[1]
    file2 = sys.argv[2]

    # データ読み込み（合算なし、x_scaled / z_scaled を付与）
    df1 = load_data_no_aggregate(file1)
    df2 = load_data_no_aggregate(file2)

    # ---- X_bin: 150–170 ----
    plot_x_range(
        df1, df2,
        start_bin=150,
        end_bin=170,
        output_filename="comparison_X_150_170.png",
        title="X-bin Comparison (150–170)"
    )

    # ---- X_bin: 220–240 ----
    plot_x_range(
        df1, df2,
        start_bin=225,
        end_bin=240,
        output_filename="comparison_X_220_240.png",
        title="X-bin Comparison (220–240)"
    )

    # ---- Z_bin: 0–20 ----
    plot_z_range(
        df1, df2,
        start_bin=0,
        end_bin=20,
        output_filename="comparison_Z_0_20.png",
        title="Z-bin Comparison (0–20)"
    )

    # ---- Z_bin: 260–280 ----
    plot_z_range(
        df1, df2,
        start_bin=280,
        end_bin=300,
        output_filename="comparison_Z_280_300.png",
        title="Z-bin Comparison (280–300)"
    )
