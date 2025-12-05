#!/usr/bin/env python3
"""
Plotting script for performance testing results.
Generates three types of visualizations from timing data.
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
from typing import List, Dict


RESULTS_FILE = Path('results/performance_results.csv')
PLOTS_DIR = Path('plots')

# Color palette for different algorithms
COLORS = {
    'exact': '#e74c3c',
    'approx1': '#3498db',
    'approx2_degree': '#2ecc71',
    'approx2_directed': '#f39c12',
    'approx2_directed_ignore': '#9b59b6',
    'approx2_histogram': '#1abc9c',
    'approx2_structure': '#e67e22',
    'approx2_greedy': '#95a5a6'
}

# Markers for different graph sets
MARKERS = {'dense_only': 'o', 'sparse_only': 's', 'dense_small_exact': '^',
           'big_dense_small_sparse': 'D', 'small_dense_big_sparse': 'v',
           'subgraph_variation': 'x'}


def load_data() -> pd.DataFrame:
    """Load performance results from CSV."""
    if not RESULTS_FILE.exists():
        raise FileNotFoundError(f"Results file not found: {RESULTS_FILE}")

    df = pd.read_csv(RESULTS_FILE)

    # Filter only successful runs
    df = df[df['status'] == 'success'].copy()

    # Create algorithm_full column combining algorithm and heuristic
    df['algorithm_full'] = df.apply(
        lambda row: f"{row['algorithm']}_{row['heuristic']}"
        if row['heuristic'] != 'N/A' else row['algorithm'],
        axis=1
    )

    return df


def plot_type1_per_graph_set(df: pd.DataFrame):
    """
    Type 1: Per-graph-set comparison.
    One plot per graph set, showing all algorithms.
    """
    print("\n📊 Generating Type 1 plots (per-graph-set comparison)...")

    # Get size variation data only
    size_df = df[df['test_type'] == 'size_variation'].copy()

    graph_sets = size_df['graph_set'].unique()

    for graph_set in graph_sets:
        set_data = size_df[size_df['graph_set'] == graph_set]

        if set_data.empty:
            continue

        fig, ax = plt.subplots(figsize=(12, 7))

        algorithms = set_data['algorithm_full'].unique()

        for algorithm in sorted(algorithms):
            algo_data = set_data[set_data['algorithm_full'] == algorithm]
            algo_data = algo_data.sort_values('pattern_size')

            color = COLORS.get(algorithm, '#34495e')

            ax.plot(
                algo_data['pattern_size'],
                algo_data['execution_time_ms'],
                marker='o',
                label=algorithm,
                color=color,
                linewidth=2,
                markersize=8,
                alpha=0.8
            )

        ax.set_xlabel('Pattern Graph Size (vertices)', fontsize=12, fontweight='bold')
        ax.set_ylabel('Execution Time (ms)', fontsize=12, fontweight='bold')
        ax.set_title(f'Algorithm Performance Comparison: {graph_set}',
                     fontsize=14, fontweight='bold', pad=20)
        ax.set_yscale('log')
        ax.grid(True, alpha=0.3, linestyle='--')
        ax.legend(loc='best', fontsize=10, framealpha=0.9)

        plt.tight_layout()
        output_file = PLOTS_DIR / f'comparison_{graph_set}.png'
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()

        print(f"  ✓ {output_file.name}")


def plot_type2_per_algorithm(df: pd.DataFrame):
    """
    Type 2: Per-algorithm comparison.
    One plot per algorithm, showing all graph sets.
    """
    print("\n📊 Generating Type 2 plots (per-algorithm comparison)...")

    # Get size variation data only
    size_df = df[df['test_type'] == 'size_variation'].copy()

    algorithms = size_df['algorithm_full'].unique()

    for algorithm in sorted(algorithms):
        algo_data = size_df[size_df['algorithm_full'] == algorithm]

        if algo_data.empty:
            continue

        fig, ax = plt.subplots(figsize=(12, 7))

        graph_sets = algo_data['graph_set'].unique()

        for graph_set in sorted(graph_sets):
            set_data = algo_data[algo_data['graph_set'] == graph_set]
            set_data = set_data.sort_values('pattern_size')

            marker = MARKERS.get(graph_set, 'o')

            ax.plot(
                set_data['pattern_size'],
                set_data['execution_time_ms'],
                marker=marker,
                label=graph_set,
                linewidth=2,
                markersize=8,
                alpha=0.8
            )

        ax.set_xlabel('Pattern Graph Size (vertices)', fontsize=12, fontweight='bold')
        ax.set_ylabel('Execution Time (ms)', fontsize=12, fontweight='bold')
        ax.set_title(f'Graph Set Performance for: {algorithm}',
                     fontsize=14, fontweight='bold', pad=20)
        ax.set_yscale('log')
        ax.grid(True, alpha=0.3, linestyle='--')
        ax.legend(loc='best', fontsize=10, framealpha=0.9)

        plt.tight_layout()
        output_file = PLOTS_DIR / f'algorithm_{algorithm}.png'
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()

        print(f"  ✓ {output_file.name}")


def plot_type3_subgraph_variation(df: pd.DataFrame):
    """
    Type 3: Subgraph count variation.
    One plot per algorithm, showing execution time vs. number of subgraphs.
    """
    print("\n📊 Generating Type 3 plots (subgraph count variation)...")

    # Get subgraph variation data only
    subgraph_df = df[df['test_type'] == 'subgraph_variation'].copy()

    if subgraph_df.empty:
        print("  ⚠️  No subgraph variation data found")
        return

    algorithms = subgraph_df['algorithm_full'].unique()

    for algorithm in sorted(algorithms):
        algo_data = subgraph_df[subgraph_df['algorithm_full'] == algorithm]

        if algo_data.empty:
            continue

        algo_data = algo_data.sort_values('num_subgraphs')

        fig, ax = plt.subplots(figsize=(10, 7))

        color = COLORS.get(algorithm, '#34495e')

        ax.plot(
            algo_data['num_subgraphs'],
            algo_data['execution_time_ms'],
            marker='o',
            color=color,
            linewidth=2.5,
            markersize=10,
            alpha=0.8
        )

        ax.set_xlabel('Number of Subgraphs to Find', fontsize=12, fontweight='bold')
        ax.set_ylabel('Execution Time (ms)', fontsize=12, fontweight='bold')
        ax.set_title(f'Subgraph Count Scaling: {algorithm}\n(Pattern=10, Target=30, Dense)',
                     fontsize=14, fontweight='bold', pad=20)
        ax.set_xticks(algo_data['num_subgraphs'])
        ax.grid(True, alpha=0.3, linestyle='--')

        plt.tight_layout()
        output_file = PLOTS_DIR / f'subgraphs_{algorithm}.png'
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()

        print(f"  ✓ {output_file.name}")


def generate_summary_stats(df: pd.DataFrame):
    """Generate and print summary statistics."""
    print("\n📈 Summary Statistics")
    print("="*60)

    # Overall stats
    total_tests = len(df)

    # Size variation stats
    size_df = df[df['test_type'] == 'size_variation']

    print(f"Total successful tests: {total_tests}")
    print(f"  Size variation tests: {len(size_df)}")
    print(f"  Subgraph variation tests: {len(df) - len(size_df)}")

    # Fastest algorithm per graph set
    if not size_df.empty:
        print("\n🏆 Fastest Algorithm by Graph Set (median time):")
        for graph_set in size_df['graph_set'].unique():
            set_data = size_df[size_df['graph_set'] == graph_set]
            median_times = set_data.groupby('algorithm_full')['execution_time_ms'].median()
            fastest = median_times.idxmin()
            fastest_time = median_times.min()
            print(f"  {graph_set:30s} -> {fastest:25s} ({fastest_time:.2f}ms)")

    # Execution time ranges
    print(f"\n⏱️  Execution Time Range:")
    print(f"  Min: {df['execution_time_ms'].min():.2f}ms")
    print(f"  Max: {df['execution_time_ms'].max():.2f}ms")
    print(f"  Mean: {df['execution_time_ms'].mean():.2f}ms")
    print(f"  Median: {df['execution_time_ms'].median():.2f}ms")


def main():
    """Generate all plots from performance results."""
    print("="*60)
    print("PERFORMANCE RESULTS VISUALIZATION")
    print("="*60)

    # Create plots directory
    PLOTS_DIR.mkdir(exist_ok=True)

    # Load data
    print(f"\n📂 Loading results from {RESULTS_FILE}...")
    try:
        df = load_data()
        print(f"  ✓ Loaded {len(df)} successful test results")
    except FileNotFoundError as e:
        print(f"\n❌ ERROR: {e}")
        print("Please run run_performance_tests.py first to generate results.")
        return 1
    except Exception as e:
        print(f"\n❌ ERROR loading data: {e}")
        return 1

    if df.empty:
        print("\n⚠️  No successful test results to plot")
        return 1

    # Generate all plot types
    plot_type1_per_graph_set(df)
    plot_type2_per_algorithm(df)
    plot_type3_subgraph_variation(df)

    # Generate summary statistics
    generate_summary_stats(df)

    print(f"\n{'='*60}")
    print(f"✓ All plots saved to {PLOTS_DIR}/")
    print(f"{'='*60}")

    # Count generated plots
    plot_count = len(list(PLOTS_DIR.glob('*.png')))
    print(f"Total plots generated: {plot_count}")

    return 0


if __name__ == '__main__':
    exit(main())
