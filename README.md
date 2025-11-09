# Subgraphs - Minimal Multigraph Extension

This repository contains implementations for solving the **Minimal Multigraph Extension** problem, including both exact and approximation algorithms.

## Overview

**Problem:** Find a minimal-cost extension to a host multigraph G such that the extended graph contains n subgraphs isomorphic to a pattern multigraph P.

**Current Implementations:**
- ✅ **Algorithm v.3**: Polynomial-time approximation algorithm (O(n × N³))
- 🚧 **Exact Algorithm**: O(k^n) time complexity (planned)

## Algorithm v.3 - Polynomial-Time Approximation

A greedy, iterative algorithm that efficiently finds approximate solutions:

### Features
- **Linear Cost Heuristic**: Uses vertex degree differences for initial approximation
- **K-Best Assignment**: Generates multiple candidate mappings to ensure uniqueness
- **Multigraph Support**: Handles edges with multiplicity > 1
- **Guaranteed Uniqueness**: Each copy uses a distinct set of vertices

### Quick Start

```bash
# Create virtual environment
python3 -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Run examples
python src/algorithm_v3.py        # Basic examples
python src/examples_v3.py          # Practical applications
python src/test_algorithm_v3.py    # Test suite
```

### Basic Usage

```python
import numpy as np
from src.algorithm_v3 import find_minimal_extension_v3

# Host graph (6 vertices)
A_G = np.array([
    [0, 1, 0, 0, 0, 0],
    [1, 0, 1, 0, 0, 0],
    [0, 1, 0, 0, 0, 0],
    [0, 0, 0, 0, 1, 0],
    [0, 0, 0, 1, 0, 1],
    [0, 0, 0, 0, 1, 0]
])

# Pattern (triangle)
A_P = np.array([
    [0, 1, 1],
    [1, 0, 1],
    [1, 1, 0]
])

# Find 2 copies
A_result, cost = find_minimal_extension_v3(A_G, A_P, n=2)
print(f"Added {cost} edges")
```

## Documentation

- **[ALGORITHM_V3_README.md](ALGORITHM_V3_README.md)**: Comprehensive documentation for Algorithm v.3
  - Problem definition and constraints
  - Algorithm strategy and complexity
  - Implementation details
  - API reference
  - Limitations and future work

## Project Structure

```
subgraphs/
├── src/
│   ├── algorithm_v3.py          # Main implementation
│   ├── test_algorithm_v3.py     # Comprehensive test suite
│   └── examples_v3.py            # Practical examples
├── requirements.txt              # Python dependencies
├── README.md                     # This file
└── ALGORITHM_V3_README.md       # Algorithm v.3 documentation
```

## Requirements

- Python 3.7+
- NumPy >= 1.20.0
- SciPy >= 1.7.0

## Examples Included

The implementation includes 5 practical examples:

1. **Social Network**: Finding friend triangles with minimal connection suggestions
2. **Road Network**: Creating alternative routes with minimal new roads
3. **Computer Network**: Adding redundant connections for fault tolerance
4. **Molecular Chemistry**: Forming ring structures with minimal bonds
5. **Custom Graph**: Template for your own applications

Run all examples:
```bash
python src/examples_v3.py
```

## Testing

Comprehensive test suite with 24+ tests covering:
- Unit tests for helper functions
- Integration tests for main algorithm
- Edge cases (empty graphs, existing patterns, etc.)
- Multigraph support
- Self-loops handling

Run tests:
```bash
python src/test_algorithm_v3.py
```

All tests passing: ✅ 24/24

## Algorithm Complexity

**Time Complexity**: O(n × N³)
- n: number of copies to find
- N: number of vertices in host graph
- Dominated by Hungarian algorithm for assignment problem

**Space Complexity**: O(N² + n × k)
- N²: adjacency matrices
- n × k: tracking used vertex sets

## Key Features

- ✅ Supports multigraphs (edges with multiplicity > 1)
- ✅ Ensures unique vertex sets for each copy
- ✅ Polynomial-time approximation
- ✅ Handles undirected graphs with symmetry
- ✅ Comprehensive test coverage
- ✅ Multiple practical examples
- ✅ Well-documented codebase

## Future Work

1. **Full Murty's Algorithm**: Replace k-best assignment heuristic with complete implementation
2. **Directed Graphs**: Extend support to directed multigraphs
3. **Exact Algorithm**: Implement O(k^n) exact solution
4. **Optimization**: Explore better approximations for large graphs
5. **Visualization**: Add graph visualization tools

## TODOs from Original Specification

- ✅ 1. Decide on graph representation → Adjacency matrices (NumPy arrays)
- ✅ 2. Decide on the metric → Edge count (extension cost)
- 🚧 3. Checking if G_s is a subgraph of G_b → Partial (can check cost == 0)
- ✅ 4. Finding minimal extension → Algorithm v.3 implemented
- ✅ 5. Approximation → O(n × N³) polynomial-time algorithm

## Contributing

This is a research project. For questions or collaboration:
- Review the documentation in `ALGORITHM_V3_README.md`
- Examine test cases in `src/test_algorithm_v3.py`
- Try the examples in `src/examples_v3.py`

## License

Educational and research purposes.

## References

- Hungarian Algorithm for Assignment Problem
- Murty's Algorithm for K-Best Assignments
- Graph Isomorphism and Subgraph Matching

---

**Status**: Algorithm v.3 is fully implemented, tested, and documented. Ready for use and further development.
