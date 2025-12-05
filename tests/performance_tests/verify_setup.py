#!/usr/bin/env python3
"""
Quick verification test - runs on a subset of graphs to verify the system works.
"""

import subprocess
import sys

def test_small_sample():
    """Run a quick test on one graph from each set."""
    test_cases = [
        ('test_graphs/dense_small_exact/graph_p5_t10.txt', '1', 'approx1', None),
        ('test_graphs/dense_small_exact/graph_p5_t10.txt', '1', 'approx2', 'degree'),
        ('test_graphs/dense_small_exact/graph_p5_t10.txt', '1', 'exact', None),
        ('test_graphs/subgraph_variation/graph_p10_t30.txt', '2', 'approx1', None),
    ]

    executable = '../../build/bin/release/subgraphs'

    print("Running quick verification tests...\n")

    for i, (graph, subgraphs, algo, heuristic) in enumerate(test_cases, 1):
        cmd = [executable, graph, subgraphs, algo]
        if heuristic:
            cmd.append(heuristic)

        algo_name = f"{algo}_{heuristic}" if heuristic else algo
        print(f"Test {i}/4: {graph.split('/')[-1]} with {algo_name}...", end=' ')

        try:
            result = subprocess.run(cmd, capture_output=True, timeout=10, text=True)
            if result.returncode == 0:
                print("✓")
            else:
                print("✗ Failed")
                print(result.stderr)
                return False
        except subprocess.TimeoutExpired:
            print("✗ Timeout")
            return False
        except Exception as e:
            print(f"✗ Error: {e}")
            return False

    print("\n✓ All verification tests passed!")
    return True

if __name__ == '__main__':
    success = test_small_sample()
    sys.exit(0 if success else 1)
