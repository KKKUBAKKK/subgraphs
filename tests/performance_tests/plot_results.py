#!/usr/bin/env python3
"""
Plotting script for POTRZEBNE.md requirements.
Generates exactly 10 plots with Polish labels.
Shows both pattern and target graph sizes.
"""

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from pathlib import Path

plt.rcParams['font.family'] = 'DejaVu Sans'
plt.rcParams['font.size'] = 12
plt.rcParams['axes.titlesize'] = 14
plt.rcParams['axes.labelsize'] = 12
plt.rcParams['legend.fontsize'] = 8
plt.rcParams['figure.dpi'] = 150

RESULTS_FILE = Path('results/performance_results.csv')
PLOTS_DIR = Path('plots')

# Polish translations for all algorithms
ALGO_NAMES = {
    'exact': 'Dokładny',
    'approx1': 'Aproksymacyjny v1',
    'approx2_degree': 'Aproks. v2 (stopień)',
    'approx2_directed': 'Aproks. v2 (kierunkowy)',
    'approx2_directed_ignore': 'Aproks. v2 (kier. ign.)',
    'approx2_histogram': 'Aproks. v2 (histogram)',
    'approx2_structure': 'Aproks. v2 (struktura)',
    'approx2_greedy': 'Aproks. v2 (zachłanny)',
}

COLORS = {
    'exact': '#c0392b',
    'approx1': '#2980b9',
    'approx2_degree': '#27ae60',
    'approx2_directed': '#f39c12',
    'approx2_directed_ignore': '#8e44ad',
    'approx2_histogram': '#16a085',
    'approx2_structure': '#d35400',
    'approx2_greedy': '#e74c3c',
}

MARKERS = ['o', 's', '^', 'D', 'v', 'p', 'h', 'x']


def load_data():
    """Load and preprocess results."""
    df = pd.read_csv(RESULTS_FILE)
    df = df[df['status'] == 'success'].copy()

    def get_algo_full(row):
        if pd.isna(row['heuristic']) or str(row['heuristic']).strip() == '':
            return row['algorithm']
        return f"{row['algorithm']}_{row['heuristic']}"

    df['algorithm_full'] = df.apply(get_algo_full, axis=1)
    # Create combined size label: "p→t"
    df['size_label'] = df.apply(lambda r: f"{r['pattern_size']}→{r['target_size']}", axis=1)
    return df


def plot_size_variation(df, graph_set, title, filename, include_exact=False):
    """Create plot showing execution time vs graph size."""
    data = df[df['graph_set'] == graph_set].copy()

    if data.empty:
        print(f"  ⚠️  No data for {graph_set}")
        return

    fig, ax = plt.subplots(figsize=(14, 8))

    algorithms = sorted(data['algorithm_full'].unique())

    for i, algo in enumerate(algorithms):
        if not include_exact and algo == 'exact':
            continue

        algo_data = data[data['algorithm_full'] == algo].sort_values('pattern_size')

        ax.plot(
            range(len(algo_data)),  # Use index for x-axis
            algo_data['execution_time_ms'],
            marker=MARKERS[i % len(MARKERS)],
            label=ALGO_NAMES.get(algo, algo),
            color=COLORS.get(algo, '#34495e'),
            linewidth=2.5,
            markersize=9,
            alpha=0.85
        )

    # Set x-axis labels to show both sizes
    unique_sizes = data[['pattern_size', 'target_size']].drop_duplicates().sort_values('pattern_size')
    x_labels = [f"{row['pattern_size']}→{row['target_size']}" for _, row in unique_sizes.iterrows()]
    ax.set_xticks(range(len(x_labels)))
    ax.set_xticklabels(x_labels, rotation=45, ha='right')

    ax.set_xlabel('Rozmiar grafu (wzorzec → cel)', fontweight='bold')
    ax.set_ylabel('Czas wykonania (ms)', fontweight='bold')
    ax.set_title(title, fontsize=14, fontweight='bold', pad=15)

    # Log scale if needed
    times = data['execution_time_ms']
    if times.max() / times.min() > 20:
        ax.set_yscale('log')

    ax.grid(True, alpha=0.3, linestyle='--')
    ax.legend(loc='upper left', fontsize=8, framealpha=0.95, title='Algorytm', ncol=2)

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

    fig, ax = plt.subplots(figsize=(12, 7))

    algorithms = sorted(data['algorithm_full'].unique())

    # Get graph sizes for title
    pattern_size = data['pattern_size'].iloc[0]
    target_size = data['target_size'].iloc[0]

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
            markersize=9,
            alpha=0.85
        )

    ax.set_xlabel('Liczba szukanych podgrafów', fontweight='bold')
    ax.set_ylabel('Czas wykonania (ms)', fontweight='bold')
    ax.set_title(f'{title}\n(graf wzorca: {pattern_size} wierz., graf celu: {target_size} wierz.)',
                 fontsize=14, fontweight='bold', pad=15)

    if data['execution_time_ms'].max() / data['execution_time_ms'].min() > 20:
        ax.set_yscale('log')

    ax.set_xticks(data['num_subgraphs'].unique())
    ax.grid(True, alpha=0.3, linestyle='--')
    ax.legend(loc='upper left', fontsize=8, framealpha=0.95, title='Algorytm', ncol=2)

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
    # WYKRESY APROKSYMACYJNE (5) - 10 punktów danych
    # =========================================================================
    print("\n" + "=" * 60)
    print("WYKRESY APROKSYMACYJNE (10 punktów danych)")
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
        'Algorytmy aproksymacyjne: Zmiana liczby podgrafów',
        '05_approx_subgraph_var.png', include_exact=False)

    # =========================================================================
    # WYKRESY DOKŁADNEGO + APROKSYMACYJNE (5) - 5 punktów danych
    # =========================================================================
    print("\n" + "=" * 60)
    print("WYKRESY DOKŁADNEGO + APROKSYMACYJNE (5 punktów danych)")
    print("=" * 60)

    print("\n📊 6. Dokładny + Aproks: grafy gęste")
    plot_size_variation(df, 'exact_dense',
        'Wszystkie algorytmy: Grafy gęste',
        '06_exact_dense.png', include_exact=True)

    print("\n📊 7. Dokładny + Aproks: grafy rzadkie")
    plot_size_variation(df, 'exact_sparse',
        'Wszystkie algorytmy: Grafy rzadkie',
        '07_exact_sparse.png', include_exact=True)

    print("\n📊 8. Dokładny + Aproks: rzadki mały + gęsty duży")
    plot_size_variation(df, 'exact_sparse_dense',
        'Wszystkie algorytmy: Rzadki wzorzec + Gęsty cel',
        '08_exact_sparse_dense.png', include_exact=True)

    print("\n📊 9. Dokładny + Aproks: gęsty mały + rzadki duży")
    plot_size_variation(df, 'exact_dense_sparse',
        'Wszystkie algorytmy: Gęsty wzorzec + Rzadki cel',
        '09_exact_dense_sparse.png', include_exact=True)

    print("\n📊 10. Dokładny + Aproks: zmiana liczby podgrafów")
    plot_subgraph_variation(df, 'exact_subgraph_var',
        'Wszystkie algorytmy: Zmiana liczby podgrafów',
        '10_exact_subgraph_var.png', include_exact=True)

    print("\n" + "=" * 60)
    print(f"✓ Wygenerowano {len(list(PLOTS_DIR.glob('*.png')))} wykresów w {PLOTS_DIR}/")
    print("=" * 60)

    return 0


if __name__ == '__main__':
    exit(main())
