#!/usr/bin/env python3
"""
Enhanced matrix generator for performance testing.
Generates multiple graph sets with varying sizes and densities.
"""

import random
import os
from pathlib import Path


def generate_multigraph_matrices(n, m, pattern_sparse=False, target_sparse=False,
                                  pattern_density=0.5, target_density=0.5,
                                  output_file=None):
    """
    Generate two adjacency matrices for directed multigraphs.

    Args:
        n (int): Number of vertices for pattern graph
        m (int): Number of vertices for target graph
        pattern_sparse (bool): Whether pattern matrix should be sparse
        target_sparse (bool): Whether target matrix should be sparse
        pattern_density (float): Density for sparse pattern (0.0-1.0)
        target_density (float): Density for sparse target (0.0-1.0)
        output_file (str): Output file path
    """

    def get_value(is_sparse, density):
        """Get random edge weight based on sparsity."""
        if is_sparse:
            if random.random() < density:
                return random.randint(1, 20)
            else:
                return 0
        else:
            return random.randint(0, 20)

    with open(output_file, 'w') as f:
        # Pattern matrix (n × n)
        f.write(f'{n}\n')
        for i in range(n):
            row = []
            for j in range(n):
                if i == j:
                    row.append(0)  # No self-loops
                else:
                    row.append(get_value(pattern_sparse, pattern_density))
            f.write(' '.join(map(str, row)) + '\n')

        # Target matrix (m × m)
        f.write(f'{m}\n')
        for i in range(m):
            row = []
            for j in range(m):
                if i == j:
                    row.append(0)  # No self-loops
                else:
                    row.append(get_value(target_sparse, target_density))
            f.write(' '.join(map(str, row)) + '\n')


def generate_graph_set(set_name, sizes, pattern_sparse, target_sparse,
                       pattern_density, target_density, target_multiplier=2):
    """Generate a set of graphs with varying sizes."""
    output_dir = Path('test_graphs') / set_name
    output_dir.mkdir(parents=True, exist_ok=True)

    print(f"Generating {set_name}...")
    for pattern_size in sizes:
        target_size = pattern_size * target_multiplier
        filename = output_dir / f'graph_p{pattern_size}_t{target_size}.txt'
        generate_multigraph_matrices(
            n=pattern_size,
            m=target_size,
            pattern_sparse=pattern_sparse,
            target_sparse=target_sparse,
            pattern_density=pattern_density,
            target_density=target_density,
            output_file=str(filename)
        )
    print(f"  ✓ Generated {len(sizes)} graphs in {output_dir}")


def generate_subgraph_variation_set():
    """Generate graphs for subgraph count variation testing."""
    output_dir = Path('test_graphs') / 'subgraph_variation'
    output_dir.mkdir(parents=True, exist_ok=True)

    print("Generating subgraph_variation...")
    # Fixed size: pattern=10, target=30, both dense
    # We only need one graph file since we'll vary the subgraph count parameter
    filename = output_dir / 'graph_p10_t30.txt'
    generate_multigraph_matrices(
        n=10,
        m=30,
        pattern_sparse=False,
        target_sparse=False,
        pattern_density=0.5,
        target_density=0.5,
        output_file=str(filename)
    )
    print(f"  ✓ Generated graph for subgraph variation in {output_dir}")


def main():
    """Generate all graph sets for performance testing."""
    random.seed(42)  # For reproducibility

    print("=" * 60)
    print("Matrix Generation for Performance Testing")
    print("=" * 60)

    # 1. Dense graphs only (both pattern and target dense)
    generate_graph_set(
        set_name='dense_only',
        sizes=[5, 10, 15, 20, 30],
        pattern_sparse=False,
        target_sparse=False,
        pattern_density=0.5,
        target_density=0.5,
        target_multiplier=2
    )

    # 2. Sparse graphs only (both pattern and target sparse)
    generate_graph_set(
        set_name='sparse_only',
        sizes=[10, 20, 30, 50, 75, 100, 150, 200],
        pattern_sparse=True,
        target_sparse=True,
        pattern_density=0.1,
        target_density=0.1,
        target_multiplier=2
    )

    # 3. Small dense graphs for exact algorithm
    generate_graph_set(
        set_name='dense_small_exact',
        sizes=[5, 8, 10, 12, 15],
        pattern_sparse=False,
        target_sparse=False,
        pattern_density=0.5,
        target_density=0.5,
        target_multiplier=2
    )

    # 4. Big dense pattern + small sparse target
    generate_graph_set(
        set_name='big_dense_small_sparse',
        sizes=[20, 30, 50],
        pattern_sparse=False,
        target_sparse=True,
        pattern_density=0.5,
        target_density=0.1,
        target_multiplier=2
    )

    # 5. Small dense pattern + big sparse target
    generate_graph_set(
        set_name='small_dense_big_sparse',
        sizes=[10, 15, 20],
        pattern_sparse=False,
        target_sparse=True,
        pattern_density=0.5,
        target_density=0.1,
        target_multiplier=4  # 4x for bigger target
    )

    # 6. Subgraph variation (fixed size, will vary subgraph count in tests)
    generate_subgraph_variation_set()

    print("=" * 60)
    print("✓ All graph sets generated successfully!")
    print("=" * 60)
    print(f"Total graph sets: 6")
    print(f"Output directory: test_graphs/")


if __name__ == '__main__':
    main()
