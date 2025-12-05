# Performance Testing Suite

Comprehensive performance testing framework for subgraph approximation algorithms.

## Overview

This suite tests all algorithm variants (exact, approx1, approx2 with 6 heuristics) across multiple graph types and sizes to analyze:
- **Performance scaling** with graph size
- **Algorithm comparison** across different graph types (dense, sparse, mixed)
- **Subgraph count impact** on execution time

## Quick Start

### 1. Setup Environment

```bash
# First time only: set up Python environment
./setup_venv.sh
```

### 2. Run Complete Test Suite

```bash
# Run everything: generate graphs, test algorithms, create plots
./run_all_tests.sh
```

This will:
1. Generate test matrices (6 graph sets, ~24 files)
2. Run performance tests (~200+ algorithm runs)
3. Create visualizations (~22 plots)

### 3. View Results

```bash
# View plots
open plots/

# View CSV data
cat results/performance_results.csv
```

## Manual Usage

### Generate Test Matrices

```bash
source venv/bin/activate
python3 matrix_gen.py
```

Generates 6 graph sets:
- `dense_only` - Both pattern and target dense (5-30 vertices)
- `sparse_only` - Both sparse (10-200 vertices)
- `dense_small_exact` - Small dense for exact algorithm (5-15 vertices)
- `big_dense_small_sparse` - Dense pattern, sparse target
- `small_dense_big_sparse` - Sparse pattern, dense target
- `subgraph_variation` - Fixed size for subgraph count testing

### Run Performance Tests

```bash
source venv/bin/activate
python3 run_performance_tests.py
```

Tests all algorithms with intelligent filtering:
- **Exact algorithm**: Only on pattern ≤15 vertices, ≤3 subgraphs
- **Approximation algorithms**: All graph sizes and subgraph counts

Results saved to `results/performance_results.csv`

### Generate Plots

```bash
source venv/bin/activate
python3 plot_results.py
```

Creates three types of visualizations:

**Type 1**: Per-graph-set comparison (6 plots)
- `comparison_{graph_set}.png`
- Shows all algorithms on one graph set

**Type 2**: Per-algorithm comparison (8 plots)
- `algorithm_{algorithm_name}.png`
- Shows one algorithm across all graph sets

**Type 3**: Subgraph count variation (8 plots)
- `subgraphs_{algorithm_name}.png`
- Shows how execution time scales with number of subgraphs

## Directory Structure

```
performance_tests/
├── venv/                          # Python virtual environment
├── test_graphs/                   # Generated test matrices
│   ├── dense_only/
│   ├── sparse_only/
│   ├── dense_small_exact/
│   ├── big_dense_small_sparse/
│   ├── small_dense_big_sparse/
│   └── subgraph_variation/
├── results/                       # CSV timing results
│   └── performance_results.csv
├── plots/                         # Generated visualizations
│   ├── comparison_*.png
│   ├── algorithm_*.png
│   └── subgraphs_*.png
├── matrix_gen.py                  # Matrix generator
├── run_performance_tests.py       # Testing script
├── plot_results.py                # Visualization script
├── setup_venv.sh                  # Environment setup
├── run_all_tests.sh               # Master script
├── requirements.txt               # Python dependencies
└── README.md                      # This file
```

## Algorithm Variants Tested

1. **exact** - Exact algorithm (optimal solution)
2. **approx1** - Approximation algorithm v1
3. **approx2** with heuristics:
   - `degree` - Degree difference
   - `directed` - Directed degree
   - `directed_ignore` - Directed degree ignoring surplus
   - `histogram` - Neighbor histogram
   - `structure` - Structure matching
   - `greedy` - Greedy neighbor

## Test Scenarios

### Graph Size Variation
Tests varying pattern graph sizes to show performance scaling.

### Subgraph Count Variation
Tests 1-5 subgraphs on fixed graph size (pattern=10, target=30) to show impact of subgraph count.

### Graph Type Combinations
- Dense graphs (both pattern and target dense)
- Sparse graphs (both sparse)
- Mixed scenarios (dense pattern + sparse target, and vice versa)

## Interpreting Results

### From Type 1 Plots (comparison_*.png)
- Compare all algorithms on a specific graph type
- Identify fastest algorithm for each graph type
- See performance curves (log scale)

### From Type 2 Plots (algorithm_*.png)
- See how one algorithm performs across different graph types
- Identify which graph types suit each algorithm best

### From Type 3 Plots (subgraphs_*.png)
- Understand how execution time scales with number of subgraphs
- Compare scalability of different algorithms

### From CSV Data
- Exact timing data for all test runs
- Status of each test (success/timeout/error/skipped)
- Full test parameters (graph sizes, algorithm, heuristic, etc.)

## Performance Expectations

- **Exact algorithm**: Exponential growth, only feasible for small graphs
- **Approximation algorithms**: Better scaling for larger graphs
- **Sparse graphs**: Generally faster than dense graphs
- **Subgraph count**: Usually linear or near-linear impact

## Troubleshooting

### Executable not found
```bash
cd ../..
./scripts/build_release.sh
```

### Tests timing out
Increase timeout in `run_performance_tests.py`:
```python
TIMEOUT_EXACT = 120  # Increase from 60
TIMEOUT_APPROX = 240  # Increase from 120
```

### Memory issues with large graphs
Reduce maximum graph sizes in `matrix_gen.py`

## Dependencies

- Python 3.8+
- matplotlib >= 3.8.0
- pandas >= 2.1.0
- numpy >= 1.26.0
- Built release version of subgraphs executable

## Notes

- All tests use random seed 42 for reproducibility
- Exact algorithm automatically skipped for pattern >15 vertices or >3 subgraphs
- Log scale used on plots for better visibility of all algorithms
- Progress indicators show completion percentage during testing
