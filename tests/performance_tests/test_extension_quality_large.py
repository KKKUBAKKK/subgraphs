#!/usr/bin/env python3
"""
Extension Quality Test on Large Graphs
Tests approximation algorithms only (no exact - too slow).
"""

import subprocess
import re
import random
import matplotlib.pyplot as plt
from pathlib import Path

plt.rcParams['font.family'] = 'DejaVu Sans'
plt.rcParams['font.size'] = 12
plt.rcParams['figure.dpi'] = 150

EXECUTABLE = '../../build/bin/release/subgraphs'
PLOTS_DIR = Path('plots')

# Approximation algorithms only
ALGORITHMS = [
    ('approx1', None, 'Aproks. v1'),
    ('approx2', 'degree', 'Aproks. v2 (stopień)'),
    ('approx2', 'directed', 'Aproks. v2 (kierunkowy)'),
    ('approx2', 'directed_ignore', 'Aproks. v2 (kier. ign.)'),
    ('approx2', 'histogram', 'Aproks. v2 (histogram)'),
    ('approx2', 'structure', 'Aproks. v2 (struktura)'),
    ('approx2', 'greedy', 'Aproks. v2 (zachłanny)'),
]

COLORS = [
    '#2980b9',  # approx1 - blue
    '#27ae60',  # degree - green
    '#f39c12',  # directed - orange
    '#8e44ad',  # directed_ignore - purple
    '#16a085',  # histogram - teal
    '#d35400',  # structure - dark orange
    '#e74c3c',  # greedy - light red
]

SUBGRAPH_COUNTS = [1, 2, 3, 4, 5]


def generate_large_graph(pattern_size, target_size, output_file, density=0.4):
    """Generate a larger test graph."""
    random.seed(42)

    with open(output_file, 'w') as f:
        # Pattern graph
        f.write(f'{pattern_size}\n')
        for i in range(pattern_size):
            row = []
            for j in range(pattern_size):
                if i == j:
                    row.append(0)
                elif random.random() < density:
                    row.append(random.randint(1, 3))
                else:
                    row.append(0)
            f.write(' '.join(map(str, row)) + '\n')

        # Target graph
        f.write(f'{target_size}\n')
        for i in range(target_size):
            row = []
            for j in range(target_size):
                if i == j:
                    row.append(0)
                elif random.random() < density:
                    row.append(random.randint(1, 3))
                else:
                    row.append(0)
            f.write(' '.join(map(str, row)) + '\n')

    return output_file


def run_algorithm(graph_file, algorithm, heuristic, num_subgraphs, timeout=120):
    """Run algorithm and return extension size."""
    cmd = [EXECUTABLE, graph_file, str(num_subgraphs), algorithm]
    if heuristic:
        cmd.append(heuristic)

    try:
        result = subprocess.run(cmd, capture_output=True, timeout=timeout, text=True)

        if result.returncode == 0:
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
    print("TEST JAKOŚCI ROZSZERZEŃ - DUŻE GRAFY")
    print("(tylko algorytmy aproksymacyjne)")
    print("=" * 60)

    PLOTS_DIR.mkdir(exist_ok=True)

    # Generate larger graph
    pattern_size = 10
    target_size = 30
    graph_file = 'test_graphs/large_quality_test.txt'
    Path('test_graphs').mkdir(exist_ok=True)

    print(f"\n📁 Generowanie grafu: wzorzec={pattern_size}, cel={target_size}...")
    generate_large_graph(pattern_size, target_size, graph_file)
    print(f"  ✓ Zapisano: {graph_file}")

    # Collect results
    results = {}

    for algo, heuristic, name in ALGORITHMS:
        results[name] = {}
        print(f"\n📊 {name}...")

        for num_subgraphs in SUBGRAPH_COUNTS:
            print(f"  Podgrafów: {num_subgraphs}...", end=' ', flush=True)

            ext_size, status = run_algorithm(graph_file, algo, heuristic, num_subgraphs)

            if status == 'success':
                results[name][num_subgraphs] = ext_size
                print(f"✓ rozszerzenie: {ext_size} krawędzi")
            else:
                results[name][num_subgraphs] = None
                print(f"✗ {status}")

    # Create the plot
    print("\n" + "=" * 60)
    print("Tworzenie wykresu...")

    fig, ax = plt.subplots(figsize=(14, 8))

    bar_width = 0.12
    x_positions = list(range(len(SUBGRAPH_COUNTS)))

    for i, (algo, heuristic, name) in enumerate(ALGORITHMS):
        extension_sizes = []
        for num_subgraphs in SUBGRAPH_COUNTS:
            size = results[name].get(num_subgraphs)
            extension_sizes.append(size if size is not None else 0)

        bar_x = [x + i * bar_width for x in x_positions]

        bars = ax.bar(bar_x, extension_sizes, bar_width,
                      label=name, color=COLORS[i], alpha=0.85, edgecolor='white')

        for bar, size in zip(bars, extension_sizes):
            if size > 0:
                ax.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.5,
                       str(size), ha='center', va='bottom', fontsize=8, fontweight='bold')

    ax.set_xlabel('Liczba szukanych podgrafów', fontweight='bold', fontsize=12)
    ax.set_ylabel('Rozmiar rozszerzenia (liczba krawędzi)', fontweight='bold', fontsize=12)
    ax.set_title(f'Jakość rozszerzeń: algorytmy aproksymacyjne\n(graf wzorca: {pattern_size} wierz., graf celu: {target_size} wierz.)',
                 fontsize=14, fontweight='bold', pad=15)

    ax.set_xticks([x + bar_width * (len(ALGORITHMS) - 1) / 2 for x in x_positions])
    ax.set_xticklabels(SUBGRAPH_COUNTS)

    ax.legend(loc='upper left', fontsize=9, framealpha=0.95, title='Algorytm')
    ax.grid(True, alpha=0.3, linestyle='--', axis='y')
    ax.set_ylim(bottom=0)

    plt.tight_layout()

    output_file = PLOTS_DIR / 'extension_quality_large.png'
    plt.savefig(output_file, dpi=200, bbox_inches='tight', facecolor='white')
    plt.close()

    print(f"✓ Zapisano: {output_file}")

    # Print summary table
    print("\n" + "=" * 60)
    print("PODSUMOWANIE WYNIKÓW")
    print("=" * 60)
    header = f"\n{'Algorytm':<30}"
    for n in SUBGRAPH_COUNTS:
        header += f" | {n} podgr."
    print(header)
    print("-" * (35 + 10 * len(SUBGRAPH_COUNTS)))

    for algo, heuristic, name in ALGORITHMS:
        row = f"{name:<30}"
        for num_subgraphs in SUBGRAPH_COUNTS:
            size = results[name].get(num_subgraphs)
            if size is not None:
                row += f" | {size:>7}"
            else:
                row += f" | {'—':>7}"
        print(row)

    print("\n" + "=" * 60)
    print("✓ Test zakończony!")
    print("=" * 60)

    return 0


if __name__ == '__main__':
    exit(main())
