#!/usr/bin/env python3
"""
Performance testing script for subgraph algorithms.
Tests all algorithm variants across multiple graph sets and measures execution times.
"""

import subprocess
import time
import csv
import os
from pathlib import Path
from typing import List, Dict, Tuple, Optional


# Configuration
EXECUTABLE_PATH = '../../build/bin/release/subgraphs'
RESULTS_DIR = Path('results')
RESULTS_FILE = RESULTS_DIR / 'performance_results.csv'
TIMEOUT_EXACT = 60  # seconds
TIMEOUT_APPROX = 120  # seconds

# Algorithm configuration
ALGORITHMS = {
    'exact': {'heuristics': [None], 'timeout': TIMEOUT_EXACT},
    'approx1': {'heuristics': [None], 'timeout': TIMEOUT_APPROX},
    'approx2': {
        'heuristics': ['degree', 'directed', 'directed_ignore', 'histogram', 'structure', 'greedy'],
        'timeout': TIMEOUT_APPROX
    }
}


def should_run_exact(pattern_size: int, num_subgraphs: int) -> bool:
    """Determine if exact algorithm should be run based on constraints."""
    return pattern_size <= 15 and num_subgraphs <= 3


def parse_graph_filename(filename: str) -> Tuple[int, int]:
    """Extract pattern and target sizes from filename like 'graph_p10_t20.txt'."""
    parts = filename.replace('.txt', '').split('_')
    pattern_size = int(parts[1][1:])  # Remove 'p' prefix
    target_size = int(parts[2][1:])    # Remove 't' prefix
    return pattern_size, target_size


def run_algorithm(graph_file: Path, algorithm: str, heuristic: Optional[str],
                  num_subgraphs: int, timeout: int) -> Tuple[Optional[float], str]:
    """
    Run a single algorithm test and measure execution time.

    Returns:
        (execution_time_ms, status) where status is 'success', 'timeout', or 'error'
    """
    cmd = [EXECUTABLE_PATH, str(graph_file), str(num_subgraphs), algorithm]
    if heuristic:
        cmd.append(heuristic)

    try:
        start_time = time.time()
        result = subprocess.run(
            cmd,
            capture_output=True,
            timeout=timeout,
            text=True
        )
        end_time = time.time()

        if result.returncode == 0:
            execution_time_ms = (end_time - start_time) * 1000
            return execution_time_ms, 'success'
        else:
            return None, f'error_code_{result.returncode}'

    except subprocess.TimeoutExpired:
        return None, 'timeout'
    except Exception as e:
        return None, f'error_{str(e)}'


def test_graph_size_variation():
    """Test performance across varying graph sizes."""
    results = []
    test_graphs_dir = Path('test_graphs')

    # Graph sets to test (excluding subgraph_variation)
    graph_sets = [
        'dense_only',
        'sparse_only',
        'dense_small_exact',
        'big_dense_small_sparse',
        'small_dense_big_sparse'
    ]

    total_tests = 0
    completed_tests = 0

    # Count total tests for progress tracking
    for set_name in graph_sets:
        set_dir = test_graphs_dir / set_name
        if not set_dir.exists():
            continue
        graph_files = sorted(set_dir.glob('*.txt'))
        for algorithm in ALGORITHMS:
            total_tests += len(graph_files) * len(ALGORITHMS[algorithm]['heuristics'])

    print(f"\n{'='*60}")
    print("Testing Graph Size Variation")
    print(f"{'='*60}")
    print(f"Total tests to run: {total_tests}\n")

    for set_name in graph_sets:
        set_dir = test_graphs_dir / set_name
        if not set_dir.exists():
            print(f"⚠️  Skipping {set_name} (directory not found)")
            continue

        print(f"\n📊 Testing {set_name}...")
        graph_files = sorted(set_dir.glob('*.txt'))

        for graph_file in graph_files:
            pattern_size, target_size = parse_graph_filename(graph_file.name)
            num_subgraphs = 1  # Default for size variation tests

            print(f"  Graph: p={pattern_size}, t={target_size}")

            for algorithm, config in ALGORITHMS.items():
                for heuristic in config['heuristics']:
                    # Skip exact algorithm if pattern too large
                    if algorithm == 'exact' and not should_run_exact(pattern_size, num_subgraphs):
                        print(f"    ⊘ Skipping {algorithm} (pattern too large)")
                        results.append({
                            'test_type': 'size_variation',
                            'graph_set': set_name,
                            'pattern_size': pattern_size,
                            'target_size': target_size,
                            'num_subgraphs': num_subgraphs,
                            'algorithm': algorithm,
                            'heuristic': heuristic or 'N/A',
                            'execution_time_ms': None,
                            'status': 'skipped'
                        })
                        completed_tests += 1
                        continue

                    algo_name = f"{algorithm}" + (f"_{heuristic}" if heuristic else "")
                    print(f"    ▶︎ Running {algo_name}...", end=' ', flush=True)

                    exec_time, status = run_algorithm(
                        graph_file, algorithm, heuristic,
                        num_subgraphs, config['timeout']
                    )

                    completed_tests += 1
                    progress = (completed_tests / total_tests) * 100

                    if status == 'success':
                        print(f"✓ {exec_time:.2f}ms [{progress:.1f}%]")
                    else:
                        print(f"✗ {status} [{progress:.1f}%]")

                    results.append({
                        'test_type': 'size_variation',
                        'graph_set': set_name,
                        'pattern_size': pattern_size,
                        'target_size': target_size,
                        'num_subgraphs': num_subgraphs,
                        'algorithm': algorithm,
                        'heuristic': heuristic or 'N/A',
                        'execution_time_ms': exec_time,
                        'status': status
                    })

    return results


def test_subgraph_count_variation():
    """Test performance across varying number of subgraphs to find."""
    results = []
    graph_file = Path('test_graphs/subgraph_variation/graph_p10_t30.txt')

    if not graph_file.exists():
        print(f"⚠️  Subgraph variation test file not found: {graph_file}")
        return results

    pattern_size, target_size = 10, 30
    subgraph_counts = [1, 2, 3, 4, 5]

    total_tests = sum(len(ALGORITHMS[alg]['heuristics']) for alg in ALGORITHMS) * len(subgraph_counts)
    completed_tests = 0

    print(f"\n{'='*60}")
    print("Testing Subgraph Count Variation")
    print(f"{'='*60}")
    print(f"Graph: p={pattern_size}, t={target_size}")
    print(f"Total tests to run: {total_tests}\n")

    for num_subgraphs in subgraph_counts:
        print(f"\n📊 Testing with {num_subgraphs} subgraph(s)...")

        for algorithm, config in ALGORITHMS.items():
            for heuristic in config['heuristics']:
                # Skip exact algorithm if subgraph count too high
                if algorithm == 'exact' and not should_run_exact(pattern_size, num_subgraphs):
                    print(f"  ⊘ Skipping {algorithm} (too many subgraphs)")
                    results.append({
                        'test_type': 'subgraph_variation',
                        'graph_set': 'subgraph_variation',
                        'pattern_size': pattern_size,
                        'target_size': target_size,
                        'num_subgraphs': num_subgraphs,
                        'algorithm': algorithm,
                        'heuristic': heuristic or 'N/A',
                        'execution_time_ms': None,
                        'status': 'skipped'
                    })
                    completed_tests += 1
                    continue

                algo_name = f"{algorithm}" + (f"_{heuristic}" if heuristic else "")
                print(f"  ▶︎ Running {algo_name}...", end=' ', flush=True)

                exec_time, status = run_algorithm(
                    graph_file, algorithm, heuristic,
                    num_subgraphs, config['timeout']
                )

                completed_tests += 1
                progress = (completed_tests / total_tests) * 100

                if status == 'success':
                    print(f"✓ {exec_time:.2f}ms [{progress:.1f}%]")
                else:
                    print(f"✗ {status} [{progress:.1f}%]")

                results.append({
                    'test_type': 'subgraph_variation',
                    'graph_set': 'subgraph_variation',
                    'pattern_size': pattern_size,
                    'target_size': target_size,
                    'num_subgraphs': num_subgraphs,
                    'algorithm': algorithm,
                    'heuristic': heuristic or 'N/A',
                    'execution_time_ms': exec_time,
                    'status': status
                })

    return results


def save_results(results: List[Dict]):
    """Save results to CSV file."""
    RESULTS_DIR.mkdir(exist_ok=True)

    if not results:
        print("⚠️  No results to save")
        return

    fieldnames = [
        'test_type', 'graph_set', 'pattern_size', 'target_size',
        'num_subgraphs', 'algorithm', 'heuristic', 'execution_time_ms', 'status'
    ]

    with open(RESULTS_FILE, 'w', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(results)

    print(f"\n✓ Results saved to {RESULTS_FILE}")
    print(f"  Total records: {len(results)}")
    success_count = sum(1 for r in results if r['status'] == 'success')
    print(f"  Successful runs: {success_count}")
    print(f"  Failed/skipped: {len(results) - success_count}")


def main():
    """Run all performance tests."""
    print("="*60)
    print("SUBGRAPH ALGORITHM PERFORMANCE TESTING")
    print("="*60)
    print(f"Executable: {EXECUTABLE_PATH}")
    print(f"Timeout (exact): {TIMEOUT_EXACT}s")
    print(f"Timeout (approx): {TIMEOUT_APPROX}s")

    # Check if executable exists
    if not os.path.exists(EXECUTABLE_PATH):
        print(f"\n❌ ERROR: Executable not found at {EXECUTABLE_PATH}")
        print("Please build the project first using:")
        print("  cd ../.. && ./scripts/build_release.sh")
        return 1

    all_results = []

    # Run graph size variation tests
    all_results.extend(test_graph_size_variation())

    # Run subgraph count variation tests
    all_results.extend(test_subgraph_count_variation())

    # Save all results
    save_results(all_results)

    print(f"\n{'='*60}")
    print("✓ Performance testing completed!")
    print(f"{'='*60}")

    return 0


if __name__ == '__main__':
    exit(main())
