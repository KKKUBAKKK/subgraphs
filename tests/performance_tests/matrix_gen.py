#!/usr/bin/env python3
"""
Matrix generator for specific performance test scenarios.
Generates graphs according to POTRZEBNE.md requirements.
"""

import random
from pathlib import Path


def generate_multigraph_matrices(n, m, pattern_sparse=False, target_sparse=False,
                                  pattern_density=0.5, target_density=0.5,
                                  output_file=None):
    """Generate two adjacency matrices for directed multigraphs."""

    def get_value(is_sparse, density):
        if is_sparse:
            return random.randint(1, 20) if random.random() < density else 0
        else:
            return random.randint(0, 20)

    with open(output_file, 'w') as f:
        # Pattern matrix (n × n)
        f.write(f'{n}\n')
        for i in range(n):
            row = [0 if i == j else get_value(pattern_sparse, pattern_density) for j in range(n)]
            f.write(' '.join(map(str, row)) + '\n')

        # Target matrix (m × m)
        f.write(f'{m}\n')
        for i in range(m):
            row = [0 if i == j else get_value(target_sparse, target_density) for j in range(m)]
            f.write(' '.join(map(str, row)) + '\n')


def generate_graph_set(set_name, configs):
    """Generate a set of graphs with given configurations."""
    output_dir = Path('test_graphs') / set_name
    output_dir.mkdir(parents=True, exist_ok=True)

    print(f"Generating {set_name}...")
    for config in configs:
        n, m = config['pattern'], config['target']
        filename = output_dir / f'graph_p{n}_t{m}.txt'
        generate_multigraph_matrices(
            n=n, m=m,
            pattern_sparse=config.get('pattern_sparse', False),
            target_sparse=config.get('target_sparse', False),
            pattern_density=config.get('pattern_density', 0.5),
            target_density=config.get('target_density', 0.5),
            output_file=str(filename)
        )
    print(f"  ✓ Generated {len(configs)} graphs in {output_dir}")


def main():
    """Generate all graph sets for required plots."""
    random.seed(42)

    print("=" * 60)
    print("Matrix Generation for Required Plots (POTRZEBNE.md)")
    print("=" * 60)

    # =========================================================================
    # APPROXIMATION ALGORITHM GRAPHS (no exact needed)
    # =========================================================================

    # 1. Approx: Dense graphs only (large sizes OK)
    generate_graph_set('approx_dense', [
        {'pattern': 10, 'target': 30, 'pattern_sparse': False, 'target_sparse': False},
        {'pattern': 20, 'target': 60, 'pattern_sparse': False, 'target_sparse': False},
        {'pattern': 30, 'target': 90, 'pattern_sparse': False, 'target_sparse': False},
        {'pattern': 40, 'target': 120, 'pattern_sparse': False, 'target_sparse': False},
        {'pattern': 50, 'target': 150, 'pattern_sparse': False, 'target_sparse': False},
    ])

    # 2. Approx: Sparse graphs only (large sizes OK)
    generate_graph_set('approx_sparse', [
        {'pattern': 10, 'target': 30, 'pattern_sparse': True, 'target_sparse': True,
         'pattern_density': 0.15, 'target_density': 0.15},
        {'pattern': 20, 'target': 60, 'pattern_sparse': True, 'target_sparse': True,
         'pattern_density': 0.15, 'target_density': 0.15},
        {'pattern': 30, 'target': 90, 'pattern_sparse': True, 'target_sparse': True,
         'pattern_density': 0.15, 'target_density': 0.15},
        {'pattern': 40, 'target': 120, 'pattern_sparse': True, 'target_sparse': True,
         'pattern_density': 0.15, 'target_density': 0.15},
        {'pattern': 50, 'target': 150, 'pattern_sparse': True, 'target_sparse': True,
         'pattern_density': 0.15, 'target_density': 0.15},
    ])

    # 3. Approx: Small sparse pattern + Large dense target
    generate_graph_set('approx_sparse_dense', [
        {'pattern': 8, 'target': 24, 'pattern_sparse': True, 'target_sparse': False,
         'pattern_density': 0.15},
        {'pattern': 12, 'target': 36, 'pattern_sparse': True, 'target_sparse': False,
         'pattern_density': 0.15},
        {'pattern': 16, 'target': 48, 'pattern_sparse': True, 'target_sparse': False,
         'pattern_density': 0.15},
        {'pattern': 20, 'target': 60, 'pattern_sparse': True, 'target_sparse': False,
         'pattern_density': 0.15},
        {'pattern': 25, 'target': 75, 'pattern_sparse': True, 'target_sparse': False,
         'pattern_density': 0.15},
    ])

    # 4. Approx: Small dense pattern + Large sparse target
    generate_graph_set('approx_dense_sparse', [
        {'pattern': 8, 'target': 24, 'pattern_sparse': False, 'target_sparse': True,
         'target_density': 0.15},
        {'pattern': 12, 'target': 36, 'pattern_sparse': False, 'target_sparse': True,
         'target_density': 0.15},
        {'pattern': 16, 'target': 48, 'pattern_sparse': False, 'target_sparse': True,
         'target_density': 0.15},
        {'pattern': 20, 'target': 60, 'pattern_sparse': False, 'target_sparse': True,
         'target_density': 0.15},
        {'pattern': 25, 'target': 75, 'pattern_sparse': False, 'target_sparse': True,
         'target_density': 0.15},
    ])

    # 5. Approx: Subgraph variation (fixed size, will test with 1-5 subgraphs)
    generate_graph_set('approx_subgraph_var', [
        {'pattern': 10, 'target': 60, 'pattern_sparse': False, 'target_sparse': False},
    ])

    # =========================================================================
    # EXACT + APPROXIMATION GRAPHS (small sizes for exact, max 15)
    # =========================================================================

    # 6. Exact: Dense graphs (max 15 vertices)
    generate_graph_set('exact_dense', [
        {'pattern': 4, 'target': 16, 'pattern_sparse': False, 'target_sparse': False},
        {'pattern': 5, 'target': 20, 'pattern_sparse': False, 'target_sparse': False},
        {'pattern': 6, 'target': 24, 'pattern_sparse': False, 'target_sparse': False},
        {'pattern': 8, 'target': 32, 'pattern_sparse': False, 'target_sparse': False},
        {'pattern': 10, 'target': 40, 'pattern_sparse': False, 'target_sparse': False},
    ])

    # 7. Exact: Sparse graphs (max 15 vertices)
    generate_graph_set('exact_sparse', [
        {'pattern': 4, 'target': 16, 'pattern_sparse': True, 'target_sparse': True,
         'pattern_density': 0.2, 'target_density': 0.2},
        {'pattern': 5, 'target': 20, 'pattern_sparse': True, 'target_sparse': True,
         'pattern_density': 0.2, 'target_density': 0.2},
        {'pattern': 6, 'target': 24, 'pattern_sparse': True, 'target_sparse': True,
         'pattern_density': 0.2, 'target_density': 0.2},
        {'pattern': 8, 'target': 32, 'pattern_sparse': True, 'target_sparse': True,
         'pattern_density': 0.2, 'target_density': 0.2},
        {'pattern': 10, 'target': 40, 'pattern_sparse': True, 'target_sparse': True,
         'pattern_density': 0.2, 'target_density': 0.2},
    ])

    # 8. Exact: Small sparse + Large dense (max 15)
    generate_graph_set('exact_sparse_dense', [
        {'pattern': 4, 'target': 16, 'pattern_sparse': True, 'target_sparse': False,
         'pattern_density': 0.2},
        {'pattern': 5, 'target': 20, 'pattern_sparse': True, 'target_sparse': False,
         'pattern_density': 0.2},
        {'pattern': 6, 'target': 24, 'pattern_sparse': True, 'target_sparse': False,
         'pattern_density': 0.2},
        {'pattern': 8, 'target': 32, 'pattern_sparse': True, 'target_sparse': False,
         'pattern_density': 0.2},
        {'pattern': 10, 'target': 40, 'pattern_sparse': True, 'target_sparse': False,
         'pattern_density': 0.2},
    ])

    # 9. Exact: Small dense + Large sparse (max 15)
    generate_graph_set('exact_dense_sparse', [
        {'pattern': 4, 'target': 16, 'pattern_sparse': False, 'target_sparse': True,
         'target_density': 0.2},
        {'pattern': 5, 'target': 20, 'pattern_sparse': False, 'target_sparse': True,
         'target_density': 0.2},
        {'pattern': 6, 'target': 24, 'pattern_sparse': False, 'target_sparse': True,
         'target_density': 0.2},
        {'pattern': 8, 'target': 32, 'pattern_sparse': False, 'target_sparse': True,
         'target_density': 0.2},
        {'pattern': 10, 'target': 40, 'pattern_sparse': False, 'target_sparse': True,
         'target_density': 0.2},
    ])

    # 10. Exact: Subgraph variation (pattern=4, target=8)
    generate_graph_set('exact_subgraph_var', [
        {'pattern': 4, 'target': 8, 'pattern_sparse': False, 'target_sparse': False},
    ])

    print("=" * 60)
    print("✓ All graph sets generated!")
    print("=" * 60)


if __name__ == '__main__':
    main()
