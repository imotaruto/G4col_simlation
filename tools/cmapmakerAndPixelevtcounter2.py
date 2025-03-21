import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys
import os
from matplotlib.colors import LogNorm

def process_and_plot_data(file_path, batch_size=10**6):
    """
    指定されたファイル (file_path) を読み込み、
    - ガンマ線のみを抽出
    - x,z の2次元ヒストグラムを作成
    - ピクセルごとのカウントをテキストファイルに書き出し
    - さらに、0カウントは黒色、それ以外はログスケールで色分けしたヒートマップを保存
    """

    # --- 出力ファイル名の定義 ---
    output_file = os.path.splitext(file_path)[0] + '_results4_corect_select_region2.txt'
    region_output_file = os.path.splitext(file_path)[0] + 'select_corect_region_counts2.txt'
    plot_file = os.path.splitext(file_path)[0] + '_log_count_plot2.png'

    # --- 指定領域の設定 (単位: cm) ---
    z_min, z_max = 0.215, 1.295    # Z範囲 (cm)
    x_min, x_max = -1.066, 1.066   # X範囲 (cm)

    # --- bin サイズ (0.036 mm ピッチ) ---
    bin_size = 0.0036
    z_edges = np.arange(z_min, z_max + bin_size, bin_size)
    x_edges = np.arange(x_min, x_max + bin_size, bin_size)

    # --- 2Dヒストグラム用配列を初期化 ---
    pixel_counts = np.zeros((len(x_edges) - 1, len(z_edges) - 1))

    print("データの処理を開始します...")

    # --- データをチャンクごとに読み込んで処理 ---
    for i, chunk in enumerate(pd.read_csv(
        file_path,
        delim_whitespace=True,
        header=None,
        names=['θ', 'φ', 'X', 'Y', 'Z', 'Particle'],
        chunksize=batch_size,
        encoding='ISO-8859-1',
        on_bad_lines='skip'
    )):
        print(f"チャンク {i+1} を処理中")

        # 列名と値の間にある "xxx=" を削除して数値・文字列に変換
        chunk['θ']        = chunk['θ'].str.split('=').str[1].astype(float)
        chunk['φ']        = chunk['φ'].str.split('=').str[1].astype(float)
        chunk['X']        = chunk['X'].str.split('=').str[1].astype(float)
        chunk['Y']        = chunk['Y'].str.split('=').str[1].astype(float)
        chunk['Z']        = chunk['Z'].str.split('=').str[1].astype(float)
        chunk['Particle'] = chunk['Particle'].str.split('=').str[1]

        # ガンマ線のみ抽出
        gamma_data = chunk[chunk['Particle'] == 'gamma']
        print(f"ガンマ粒子でフィルタリング: {len(gamma_data)} 行")

        # 指定領域内 (x_min <= X <= x_max, z_min <= Z <= z_max) のみ抽出
        region_gamma_data = gamma_data[
            (gamma_data['X'] >= x_min) & (gamma_data['X'] <= x_max) &
            (gamma_data['Z'] >= z_min) & (gamma_data['Z'] <= z_max)
        ]

        # 2次元ヒストグラムを計算して足しこむ
        H, _, _ = np.histogram2d(
            region_gamma_data['X'],
            region_gamma_data['Z'],
            bins=[x_edges, z_edges]
        )
        pixel_counts += H

    print("すべてのチャンクの処理が終了しました。")

    # --- ピクセルごとのガンマ線カウントをテキストファイルに保存 ---
    with open(region_output_file, 'w') as f:
        f.write("X_bin\tZ_bin\tCount\n")
        for ix in range(pixel_counts.shape[0]):
            for iz in range(pixel_counts.shape[1]):
                f.write(f"{ix}\t{iz}\t{int(pixel_counts[ix, iz])}\n")

    print(f"ピクセルごとのガンマ線のカウントが {region_output_file} に保存されました。")

    # --- ヒートマップを作成・保存 ---
    plot_log_count(pixel_counts, plot_file)

def plot_log_count(pixel_counts, plot_file):
    """
    pixel_counts: shape = (nx, nz)
      - nx: X 方向ピクセル数
      - nz: Z 方向ピクセル数

    カウント0は黒、それ以外はログスケールで着色したヒートマップを保存。
    """
    nx, nz = pixel_counts.shape

    # デバッグ用に最大カウント表示
    max_count = pixel_counts.max()
    print(f"最大カウント: {max_count}")

    # データのアスペクト比に基づいてフィギュアサイズを設定
    aspect_ratio = nz / nx
    fig_width = 12  # 任意の幅 (インチ単位)
    fig_height = fig_width * aspect_ratio

    plt.figure(figsize=(fig_width, fig_height))

    # 0 のピクセルをマスクしてログスケール表示
    masked_counts = np.ma.masked_equal(pixel_counts, 0)

    # imshow で可視化
    im = plt.imshow(
        masked_counts.T,       # 転置して (Z, X) -> (行, 列) に合わせる
        origin='lower',
        extent=[0, nx, 0, nz],  # 軸をピクセル数で表現
        cmap='inferno',
        aspect='equal',          # アスペクト比をデータに合わせる
        norm=LogNorm(vmin=1, vmax=max(1, max_count))
    )
    im.cmap.set_bad('black')   # マスクされた部分(=0)を黒で表示

    # カラーバーを追加
    cbar = plt.colorbar(im)
    cbar.set_label('Count (log scale)', fontsize=20)
    cbar.ax.tick_params(labelsize=12)

    # 軸ラベル
    plt.xlabel('X_Pixel', fontsize=30)
    plt.ylabel('Z_Pixel', fontsize=30)
    plt.xticks(fontsize=25)
    plt.yticks(fontsize=25)

    # タイトルは空
    plt.title('', fontsize=16)

    # 画像として保存
    plt.savefig(plot_file, bbox_inches='tight')
    plt.close()
    print(f"0カウントを黒、ログスケールでプロットした画像を {plot_file} に保存しました。")

# メイン処理
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <input_file>")
        sys.exit(1)
    file_path = sys.argv[1]
    process_and_plot_data(file_path)
