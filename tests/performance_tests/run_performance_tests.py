#!/usr/bin/env python3
"""
Performance testing script for specific plot requirements.
Tests algorithms according to POTRZEBNE.md.
"""

import subprocess
import time
import csv
import os
from pathlib import Path
from typing import List, Dict, Tuple, Optional


EXECUTABLE_PATH = '../../build/bin/release/subgraphs'
RESULTS_DIR = Path('results')
RESULTS_FILE = RESULTS_DIR / 'performance_results.csv'
TIMEOUT_EXACT = 120  # Longer timeout for exact
TIMEOUT_APPROX = 60

# Approximation algorithms only (no exact)
APPROX_ALGORITHMS = {
    'approx1': {'heuristics': [None], 'timeout': TIMEOUT_APPROX},
    'approx2': {
        'heuristics': ['degree', 'directed', 'directed_ignore', 'histogram', 'structure'],
        'timeout': TIMEOUT_APPROX
    }
}

# All algorithms including exact
ALL_ALGORITHMS = {
    'exact': {'heuristics': [None], 'timeout': TIMEOUT_EXACT},
    **APPROX_ALGORITHMS
}


def parse_graph_filename(filename: str) -> Tuple[int, int]:
    """Extract pattern and target sizes from filename."""
    parts = filename.replace('.txt', '').split('_')
    pattern_size = int(parts[1][1:])
    target_size = int(parts[2][1:])
    return pattern_size, target_size


def run_algorithm(graph_file: Path, algorithm: str, heuristic: Optional[str],
                  num_subgraphs: int, timeout: int) -> Tuple[Optional[float], str]:
    """Run algorithm and return (execution_time_ms, status)."""
    cmd = [EXECUTABLE_PATH, str(graph_file), str(num_subgraphs), algorithm]
    if heuristic:
        cmd.append(heuristic)

    try:
        start_time = time.time()
        result = subprocess.run(cmd, capture_output=True, timeout=timeout, text=True)
        end_time = time.time()

        if result.returncode == 0:
            return (end_time - start_time) * 1000, 'success'
        elif result.returncode < 0:
            return None, f'crash_signal_{-result.returncode}'
        else:
            return None, f'error_code_{result.returncode}'
    except subprocess.TimeoutExpired:
        return None, 'timeout'
    except Exception as e:
        return None, f'exception'


def test_graph_set(set_name: str, algorithms: dict, num_subgraphs: int = 1) -> List[Dict]:
    """Test a graph set with given algorithms."""
    results = []
    set_dir = Path('test_graphs') / set_name

    if not set_dir.exists():
        print(f"  ⚠️  Skipping {set_name} (not found)")
        return results

    graph_files = sorted(set_dir.glob('*.txt'), key=lambda x: parse_graph_filename(x.name)[0])

    for graph_file in graph_files:
        pattern_size, target_size = parse_graph_filename(graph_file.name)
        print(f"  Graph: p={pattern_size}, t={target_size}, subgraphs={num_subgraphs}")

        for algorithm, config in algorithms.items():
            for heuristic in config['heuristics']:
                algo_name = f"{algorithm}" + (f"_{heuristic}" if heuristic else "")
                print(f"    ▶︎ {algo_name}...", end=' ', flush=True)

                exec_time, status = run_algorithm(
                    graph_file, algorithm, heuristic, num_subgraphs, config['timeout']
                )

                if status == 'success':
                    print(f"✓ {exec_time:.2f}ms")
                else:
                    print(f"✗ {status}")

                results.append({
                    'graph_set': set_name,
                    'pattern_size': pattern_size,
                    'target_size': target_size,
                    'num_subgraphs': num_subgraphs,
                    'algorithm': algorithm,
                    'heuristic': heuristic if heuristic else '',
                    'execution_time_ms': exec_time,
                    'status': status
                })

    return results


def test_subgraph_variation(set_name: str, algorithms: dict, subgraph_counts: List[int]) -> List[Dict]:
    """Test varying subgraph counts on a single graph."""
    results = []
    set_dir = Path('test_graphs') / set_name

    if not set_dir.exists():
        print(f"  ⚠️  Skipping {set_name} (not found)")
        return results

    graph_file = list(set_dir.glob('*.txt'))[0]
    pattern_size, target_size = parse_graph_filename(graph_file.name)

    for num_subgraphs in subgraph_counts:
        print(f"  Subgraphs: {num_subgraphs}")

        for algorithm, config in algorithms.items():
            for heuristic in config['heuristics']:
                algo_name = f"{algorithm}" + (f"_{heuristic}" if heuristic else "")
                print(f"    ▶︎ {algo_name}...", end=' ', flush=True)

                exec_time, status = run_algorithm(
                    graph_file, algorithm, heuristic, num_subgraphs, config['timeout']
                )

                if status == 'success':
                    print(f"✓ {exec_time:.2f}ms")
                else:
                    print(f"✗ {status}")

                results.append({
                    'graph_set': set_name,
                    'pattern_size': pattern_size,
                    'target_size': target_size,
                    'num_subgraphs': num_subgraphs,
                    'algorithm': algorithm,
                    'heuristic': heuristic if heuristic else '',
                    'execution_time_ms': exec_time,
                    'status': status
                })

    return results


def save_results(results: List[Dict]):
    """Save results to CSV."""
    RESULTS_DIR.mkdir(exist_ok=True)

    fieldnames = ['graph_set', 'pattern_size', 'target_size', 'num_subgraphs',
                  'algorithm', 'heuristic', 'execution_time_ms', 'status']

    with open(RESULTS_FILE, 'w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(results)

    success = sum(1 for r in results if r['status'] == 'success')
    print(f"\n✓ Results saved: {success}/{len(results)} successful")


def main():
    print("=" * 60)
    print("PERFORMANCE TESTING (POTRZEBNE.md)")
    print("=" * 60)

    if not os.path.exists(EXECUTABLE_PATH):
        print(f"❌ Executable not found: {EXECUTABLE_PATH}")
        return 1

    all_results = []

    # =========================================================================
    # APPROXIMATION ONLY TESTS (5 scenarios)
    # =========================================================================
    print("\n" + "=" * 60)
    print("WYKRESY APROKSYMACYJNE")
    print("=" * 60)

    print("\n📊 1. Approx na grafach gęstych...")
    all_results.extend(test_graph_set('approx_dense', APPROX_ALGORITHMS))

    print("\n📊 2. Approx na grafach rzadkich...")
    all_results.extend(test_graph_set('approx_sparse', APPROX_ALGORITHMS))

    print("\n📊 3. Approx: rzadki mały + gęsty duży...")
    all_results.extend(test_graph_set('approx_sparse_dense', APPROX_ALGORITHMS))

    print("\n📊 4. Approx: gęsty mały + rzadki duży...")
    all_results.extend(test_graph_set('approx_dense_sparse', APPROX_ALGORITHMS))

    print("\n📊 5. Approx: zmiana liczby podgrafów...")
    all_results.extend(test_subgraph_variation('approx_subgraph_var', APPROX_ALGORITHMS, [1, 2, 3, 4, 5, 6]))

    # =========================================================================
    # EXACT + APPROXIMATION TESTS (5 scenarios)
    # =========================================================================
    print("\n" + "=" * 60)
    print("WYKRESY DOKŁADNEGO + APROKSYMACYJNE")
    print("=" * 60)

    print("\n📊 6. Exact + Approx na grafach gęstych (max 15)...")
    all_results.extend(test_graph_set('exact_dense', ALL_ALGORITHMS))

    print("\n📊 7. Exact + Approx na grafach rzadkich (max 15)...")
    all_results.extend(test_graph_set('exact_sparse', ALL_ALGORITHMS))

    print("\n📊 8. Exact + Approx: rzadki mały + gęsty duży (max 15)...")
    all_results.extend(test_graph_set('exact_sparse_dense', ALL_ALGORITHMS))

    print("\n📊 9. Exact + Approx: gęsty mały + rzadki duży (max 15)...")
    all_results.extend(test_graph_set('exact_dense_sparse', ALL_ALGORITHMS))

    print("\n📊 10. Exact + Approx: zmiana liczby podgrafów (p=4, t=8)...")
    all_results.extend(test_subgraph_variation('exact_subgraph_var', ALL_ALGORITHMS, [1, 2]))

    save_results(all_results)

    print("\n" + "=" * 60)
    print("✓ Testing completed!")
    print("=" * 60)

    return 0


if __name__ == '__main__':
    exit(main())
