#!/usr/bin/env python3
"""
Extension Quality Test
Tests all algorithms on sample_graphs2.txt and compares extension sizes.
Creates a plot showing how big extensions are found by each algorithm.
"""

import subprocess
import re
import matplotlib.pyplot as plt
from pathlib import Path

plt.rcParams['font.family'] = 'DejaVu Sans'
plt.rcParams['font.size'] = 12
plt.rcParams['figure.dpi'] = 150

EXECUTABLE = '../../build/bin/release/subgraphs'
GRAPH_FILE = 'sample_graphs2.txt'
PLOTS_DIR = Path('plots')

# All algorithms to test
ALGORITHMS = [
    ('exact', None, 'Dokładny'),
    ('approx1', None, 'Aproks. v1'),
    ('approx2', 'degree', 'Aproks. v2 (stopień)'),
    ('approx2', 'directed', 'Aproks. v2 (kierunkowy)'),
    ('approx2', 'directed_ignore', 'Aproks. v2 (kier. ign.)'),
    ('approx2', 'histogram', 'Aproks. v2 (histogram)'),
    ('approx2', 'structure', 'Aproks. v2 (struktura)'),
    ('approx2', 'greedy', 'Aproks. v2 (zachłanny)'),
]

COLORS = [
    '#c0392b',  # exact - red
    '#2980b9',  # approx1 - blue
    '#27ae60',  # degree - green
    '#f39c12',  # directed - orange
    '#8e44ad',  # directed_ignore - purple
    '#16a085',  # histogram - teal
    '#d35400',  # structure - dark orange
    '#e74c3c',  # greedy - light red
]

SUBGRAPH_COUNTS = [1, 2, 3]


def run_algorithm(graph_file, algorithm, heuristic, num_subgraphs, timeout=300):
    """Run algorithm and return extension size."""
    cmd = [EXECUTABLE, graph_file, str(num_subgraphs), algorithm]
    if heuristic:
        cmd.append(heuristic)

    try:
        result = subprocess.run(cmd, capture_output=True, timeout=timeout, text=True)

        if result.returncode == 0:
            # Parse "Total extension cost: X edge(s)"
            match = re.search(r'Total extension cost:\s*(\d+)', result.stdout)
            if match:
                return int(match.group(1)), 'success'
        elif result.returncode < 0:
            return None, f'crash_{-result.returncode}'
        else:
            return None, f'error_{result.returncode}'
    except subprocess.TimeoutExpired:
        return None, 'timeout'
    except Exception as e:
        return None, f'exception: {e}'

    return None, 'no_result'


def main():
    print("=" * 60)
    print("TEST JAKOŚCI ROZSZERZEŃ (sample_graphs2.txt)")
    print("=" * 60)

    PLOTS_DIR.mkdir(exist_ok=True)

    # Collect results: {algo_name: {num_subgraphs: extension_size}}
    results = {}

    for algo, heuristic, name in ALGORITHMS:
        results[name] = {}
        print(f"\n📊 {name}...")

        for num_subgraphs in SUBGRAPH_COUNTS:
            print(f"  Podgrafów: {num_subgraphs}...", end=' ', flush=True)

            ext_size, status = run_algorithm(GRAPH_FILE, algo, heuristic, num_subgraphs)

            if status == 'success':
                results[name][num_subgraphs] = ext_size
                print(f"✓ rozszerzenie: {ext_size} krawędzi")
            else:
                results[name][num_subgraphs] = None
                print(f"✗ {status}")

    # Create the plot
    print("\n" + "=" * 60)
    print("Tworzenie wykresu...")

    fig, ax = plt.subplots(figsize=(12, 8))

    bar_width = 0.1
    x_positions = list(range(len(SUBGRAPH_COUNTS)))

    for i, (algo, heuristic, name) in enumerate(ALGORITHMS):
        extension_sizes = []
        for num_subgraphs in SUBGRAPH_COUNTS:
            size = results[name].get(num_subgraphs)
            extension_sizes.append(size if size is not None else 0)

        # Offset bars for each algorithm
        bar_x = [x + i * bar_width for x in x_positions]

        bars = ax.bar(bar_x, extension_sizes, bar_width,
                      label=name, color=COLORS[i], alpha=0.85, edgecolor='white')

        # Add value labels on bars
        for bar, size in zip(bars, extension_sizes):
            if size > 0:
                ax.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.3,
                       str(size), ha='center', va='bottom', fontsize=9, fontweight='bold')

    # Configure axes
    ax.set_xlabel('Liczba szukanych podgrafów', fontweight='bold', fontsize=12)
    ax.set_ylabel('Rozmiar rozszerzenia (liczba krawędzi)', fontweight='bold', fontsize=12)
    ax.set_title('Jakość rozszerzeń: porównanie algorytmów\n(graf wzorca: 6 wierz., graf celu: 8 wierz.)',
                 fontsize=14, fontweight='bold', pad=15)

    # Center x-axis ticks
    ax.set_xticks([x + bar_width * (len(ALGORITHMS) - 1) / 2 for x in x_positions])
    ax.set_xticklabels(SUBGRAPH_COUNTS)

    ax.legend(loc='upper left', fontsize=9, framealpha=0.95, title='Algorytm')
    ax.grid(True, alpha=0.3, linestyle='--', axis='y')
    ax.set_ylim(bottom=0)

    plt.tight_layout()

    output_file = PLOTS_DIR / 'extension_quality.png'
    plt.savefig(output_file, dpi=200, bbox_inches='tight', facecolor='white')
    plt.close()

    print(f"✓ Zapisano: {output_file}")

    # Print summary table
    print("\n" + "=" * 60)
    print("PODSUMOWANIE WYNIKÓW")
    print("=" * 60)
    print(f"\n{'Algorytm':<30} | {'1 podgraf':>10} | {'2 podgrafy':>10} | {'3 podgrafy':>10}")
    print("-" * 70)

    for algo, heuristic, name in ALGORITHMS:
        row = f"{name:<30}"
        for num_subgraphs in SUBGRAPH_COUNTS:
            size = results[name].get(num_subgraphs)
            if size is not None:
                row += f" | {size:>10}"
            else:
                row += f" | {'—':>10}"
        print(row)

    print("\n" + "=" * 60)
    print("✓ Test zakończony!")
    print("=" * 60)

    return 0


if __name__ == '__main__':
    exit(main())
