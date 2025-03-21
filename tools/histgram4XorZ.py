import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys
import matplotlib as mpl

# グローバルなフォント設定をリセット（必要に応じて）
mpl.rcParams.update(mpl.rcParamsDefault)

def load_and_aggregate_data(file_path):
    # データの読み込み
    data = pd.read_csv(file_path, delim_whitespace=True)
    
    # 各Z_binのカウント数を全てのX_binで合計し、592で割る
    z_aggregated_counts = data.groupby('Z_bin')['Count'].sum() / 592
    
    # 各X_binのカウント数を全てのZ_binで合計し、300で割る
    x_aggregated_counts = data.groupby('X_bin')['Count'].sum() / 300
    
    return x_aggregated_counts, z_aggregated_counts

def plot_aggregated_counts(x_counts, z_counts):
    # フォントサイズの設定
    axis_label_fontsize = 25
    title_fontsize = 30
    tick_label_fontsize = 25

    # X_binごとの合計カウント数のプロット（ログスケール）
    fig, ax = plt.subplots(figsize=(12, 6))
    ax.bar(x_counts.index, x_counts.values, color='blue', alpha=0.7)
    ax.set_yscale('log')
    ax.set_ylim(1e-7, 1e3)  # 縦軸の範囲を 10^-2 ~ 10^5 に設定
    ax.set_xlabel('X_Pixel', fontsize=axis_label_fontsize, fontname='Arial')
    ax.set_ylabel('Counts/Pixel', fontsize=axis_label_fontsize, fontname='Arial')
    ax.set_title('', fontsize=title_fontsize, fontname='Arial')
    ax.tick_params(axis='x', labelsize=tick_label_fontsize)
    ax.tick_params(axis='y', labelsize=tick_label_fontsize)
    ax.grid(True, which="both", linestyle='--', linewidth=0.5, color='gray')
    fig.tight_layout()
    fig.savefig('x_bin_total_counts_scaled.png')
    plt.close(fig)

    # Z_binごとの合計カウント数のプロット（縦軸をlogスケールで範囲1～5000に設定）
    fig, ax = plt.subplots(figsize=(12, 6))
    ax.bar(z_counts.index, z_counts.values, color='green', alpha=0.7)
    ax.set_yscale('log')
    ax.set_xlabel('Z_Pixel', fontsize=axis_label_fontsize, fontname='Arial')
    ax.set_ylabel('Counts/Pixel', fontsize=axis_label_fontsize, fontname='Arial')
    ax.set_title('', fontsize=title_fontsize, fontname='Arial')
    ax.set_ylim(1, 5000)  # 縦軸の範囲を 1 ~ 5000 に設定
    ax.tick_params(axis='x', labelsize=tick_label_fontsize)
    ax.tick_params(axis='y', labelsize=tick_label_fontsize)
    ax.grid(True, which="both", linestyle='--', linewidth=0.5, color='gray')
    fig.tight_layout()
    fig.savefig('z_bin_total_counts_scaled.png')
    plt.close(fig)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <input_file>")
    else:
        file_path = sys.argv[1]
        x_aggregated_counts, z_aggregated_counts = load_and_aggregate_data(file_path)
        plot_aggregated_counts(x_aggregated_counts, z_aggregated_counts)
