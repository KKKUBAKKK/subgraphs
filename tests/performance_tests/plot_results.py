#!/usr/bin/env python3
"""
Plotting script for POTRZEBNE.md requirements.
Generates exactly 10 plots with Polish labels.
"""

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from pathlib import Path

plt.rcParams['font.family'] = 'DejaVu Sans'
plt.rcParams['font.size'] = 12
plt.rcParams['axes.titlesize'] = 14
plt.rcParams['axes.labelsize'] = 12
plt.rcParams['legend.fontsize'] = 9
plt.rcParams['figure.dpi'] = 150

RESULTS_FILE = Path('results/performance_results.csv')
PLOTS_DIR = Path('plots')

# Polish translations
ALGO_NAMES = {
    'exact': 'Dokładny',
    'approx1': 'Aproksymacyjny v1',
    'approx2_degree': 'Aproks. v2 (stopień)',
    'approx2_directed': 'Aproks. v2 (kierunkowy)',
    'approx2_directed_ignore': 'Aproks. v2 (kier. ignoruj)',
    'approx2_histogram': 'Aproks. v2 (histogram)',
    'approx2_structure': 'Aproks. v2 (struktura)',
}

COLORS = {
    'exact': '#c0392b',
    'approx1': '#2980b9',
    'approx2_degree': '#27ae60',
    'approx2_directed': '#f39c12',
    'approx2_directed_ignore': '#8e44ad',
    'approx2_histogram': '#16a085',
    'approx2_structure': '#d35400',
}

MARKERS = ['o', 's', '^', 'D', 'v', 'p', 'h']


def load_data():
    """Load and preprocess results."""
    df = pd.read_csv(RESULTS_FILE)
    df = df[df['status'] == 'success'].copy()

    def get_algo_full(row):
        if pd.isna(row['heuristic']) or str(row['heuristic']).strip() == '':
            return row['algorithm']
        return f"{row['algorithm']}_{row['heuristic']}"

    df['algorithm_full'] = df.apply(get_algo_full, axis=1)
    return df


def plot_size_variation(df, graph_set, title, filename, include_exact=False):
    """Create plot showing execution time vs graph size."""
    data = df[df['graph_set'] == graph_set].copy()

    if data.empty:
        print(f"  ⚠️  No data for {graph_set}")
        return

    fig, ax = plt.subplots(figsize=(12, 8))

    algorithms = sorted(data['algorithm_full'].unique())

    for i, algo in enumerate(algorithms):
        if not include_exact and algo == 'exact':
            continue

        algo_data = data[data['algorithm_full'] == algo].sort_values('pattern_size')

        ax.plot(
            algo_data['pattern_size'],
            algo_data['execution_time_ms'],
            marker=MARKERS[i % len(MARKERS)],
            label=ALGO_NAMES.get(algo, algo),
            color=COLORS.get(algo, '#34495e'),
            linewidth=2.5,
            markersize=10,
            alpha=0.85
        )

    ax.set_xlabel('Rozmiar grafu wzorca (liczba wierzchołków)', fontweight='bold')
    ax.set_ylabel('Czas wykonania (ms)', fontweight='bold')
    ax.set_title(title, fontsize=14, fontweight='bold', pad=15)

    # Log scale if needed
    if data['execution_time_ms'].max() / data['execution_time_ms'].min() > 20:
        ax.set_yscale('log')

    ax.grid(True, alpha=0.3, linestyle='--')
    ax.legend(loc='upper left', fontsize=10, framealpha=0.95, title='Algorytm')

    plt.tight_layout()
    plt.savefig(PLOTS_DIR / filename, dpi=200, bbox_inches='tight', facecolor='white')
    plt.close()
    print(f"  ✓ {filename}")


def plot_subgraph_variation(df, graph_set, title, filename, include_exact=False):
    """Create plot showing execution time vs number of subgraphs."""
    data = df[df['graph_set'] == graph_set].copy()

    if data.empty:
        print(f"  ⚠️  No data for {graph_set}")
        return

    fig, ax = plt.subplots(figsize=(11, 7))

    algorithms = sorted(data['algorithm_full'].unique())

    for i, algo in enumerate(algorithms):
        if not include_exact and algo == 'exact':
            continue

        algo_data = data[data['algorithm_full'] == algo].sort_values('num_subgraphs')

        ax.plot(
            algo_data['num_subgraphs'],
            algo_data['execution_time_ms'],
            marker=MARKERS[i % len(MARKERS)],
            label=ALGO_NAMES.get(algo, algo),
            color=COLORS.get(algo, '#34495e'),
            linewidth=2.5,
            markersize=10,
            alpha=0.85
        )

    ax.set_xlabel('Liczba szukanych podgrafów', fontweight='bold')
    ax.set_ylabel('Czas wykonania (ms)', fontweight='bold')
    ax.set_title(title, fontsize=14, fontweight='bold', pad=15)

    if data['execution_time_ms'].max() / data['execution_time_ms'].min() > 20:
        ax.set_yscale('log')

    ax.set_xticks(data['num_subgraphs'].unique())
    ax.grid(True, alpha=0.3, linestyle='--')
    ax.legend(loc='upper left', fontsize=10, framealpha=0.95, title='Algorytm')

    plt.tight_layout()
    plt.savefig(PLOTS_DIR / filename, dpi=200, bbox_inches='tight', facecolor='white')
    plt.close()
    print(f"  ✓ {filename}")


def main():
    print("=" * 60)
    print("GENEROWANIE WYKRESÓW (POTRZEBNE.md)")
    print("=" * 60)

    PLOTS_DIR.mkdir(exist_ok=True)

    print(f"\n📂 Wczytywanie z {RESULTS_FILE}...")
    try:
        df = load_data()
        print(f"  ✓ Wczytano {len(df)} wyników")
    except Exception as e:
        print(f"❌ Błąd: {e}")
        return 1

    # =========================================================================
    # WYKRESY APROKSYMACYJNE (5)
    # =========================================================================
    print("\n" + "=" * 60)
    print("WYKRESY APROKSYMACYJNE")
    print("=" * 60)

    print("\n📊 1. Aproksymacyjne na grafach gęstych")
    plot_size_variation(df, 'approx_dense',
        'Algorytmy aproksymacyjne: Grafy gęste',
        '01_approx_dense.png', include_exact=False)

    print("\n📊 2. Aproksymacyjne na grafach rzadkich")
    plot_size_variation(df, 'approx_sparse',
        'Algorytmy aproksymacyjne: Grafy rzadkie',
        '02_approx_sparse.png', include_exact=False)

    print("\n📊 3. Aproksymacyjne: rzadki mały + gęsty duży")
    plot_size_variation(df, 'approx_sparse_dense',
        'Algorytmy aproksymacyjne: Rzadki wzorzec + Gęsty cel',
        '03_approx_sparse_dense.png', include_exact=False)

    print("\n📊 4. Aproksymacyjne: gęsty mały + rzadki duży")
    plot_size_variation(df, 'approx_dense_sparse',
        'Algorytmy aproksymacyjne: Gęsty wzorzec + Rzadki cel',
        '04_approx_dense_sparse.png', include_exact=False)

    print("\n📊 5. Aproksymacyjne: zmiana liczby podgrafów")
    plot_subgraph_variation(df, 'approx_subgraph_var',
        'Algorytmy aproksymacyjne: Zmiana liczby podgrafów\n(wzorzec: 10, cel: 60 wierzchołków)',
        '05_approx_subgraph_var.png', include_exact=False)

    # =========================================================================
    # WYKRESY DOKŁADNEGO + APROKSYMACYJNE (5)
    # =========================================================================
    print("\n" + "=" * 60)
    print("WYKRESY DOKŁADNEGO + APROKSYMACYJNE")
    print("=" * 60)

    print("\n📊 6. Dokładny + Aproks: grafy gęste (max 15)")
    plot_size_variation(df, 'exact_dense',
        'Wszystkie algorytmy: Grafy gęste (max 15 wierzchołków)',
        '06_exact_dense.png', include_exact=True)

    print("\n📊 7. Dokładny + Aproks: grafy rzadkie (max 15)")
    plot_size_variation(df, 'exact_sparse',
        'Wszystkie algorytmy: Grafy rzadkie (max 15 wierzchołków)',
        '07_exact_sparse.png', include_exact=True)

    print("\n📊 8. Dokładny + Aproks: rzadki mały + gęsty duży (max 15)")
    plot_size_variation(df, 'exact_sparse_dense',
        'Wszystkie algorytmy: Rzadki wzorzec + Gęsty cel (max 15)',
        '08_exact_sparse_dense.png', include_exact=True)

    print("\n📊 9. Dokładny + Aproks: gęsty mały + rzadki duży (max 15)")
    plot_size_variation(df, 'exact_dense_sparse',
        'Wszystkie algorytmy: Gęsty wzorzec + Rzadki cel (max 15)',
        '09_exact_dense_sparse.png', include_exact=True)

    print("\n📊 10. Dokładny + Aproks: zmiana liczby podgrafów (p=4, t=8)")
    plot_subgraph_variation(df, 'exact_subgraph_var',
        'Wszystkie algorytmy: Zmiana liczby podgrafów\n(wzorzec: 4, cel: 8 wierzchołków)',
        '10_exact_subgraph_var.png', include_exact=True)

    print("\n" + "=" * 60)
    print(f"✓ Wygenerowano {len(list(PLOTS_DIR.glob('*.png')))} wykresów w {PLOTS_DIR}/")
    print("=" * 60)

    return 0


if __name__ == '__main__':
    exit(main())
