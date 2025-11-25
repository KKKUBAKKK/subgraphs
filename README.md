# Subgraph Extension Algorithm

A C++20 templated library for finding minimal extensions of directed multigraphs. Given a pattern graph $P$ and a target graph $G$, the algorithm determines the minimum set of edges that must be added to $G$ so that it contains $m$ disjoint copies of $P$ as subgraphs.

The library provides three algorithm implementations:
- **Exact algorithm**: Finds the optimal solution with exhaustive search
- **Approximation algorithm v1**: Fast heuristic-based approach
- **Approximation algorithm v2**: Advanced approximation with 6 configurable heuristics

## Problem Definition

**Input:**
- Pattern graph $P$ with $n$ vertices
- Target graph $G$ with $k$ vertices (where $k \geq n$)
- Number of copies $m$ to find
- Algorithm type: `exact`, `approx1`, or `approx2` (optional)
- Heuristic type: for `approx2` algorithm (optional)

**Output:**
- Minimal (or near-minimal) set of edges to add to $G$ such that it contains at least $m$ vertex-disjoint copies of $P$

**Complexity:**
- Exact algorithm: $O(k^n \cdot \binom{k}{n})$ for exhaustive search over all possible vertex mappings
- Approximation algorithms: Significantly faster, with trade-offs in solution quality

## Features

- **Multiple algorithm implementations**:
  - Exact algorithm for optimal solutions
  - Approximation algorithm v1 for faster results
  - Approximation algorithm v2 with 6 configurable heuristics
- **Advanced heuristics**: Degree-based, directed degree, histogram, structure matching, and greedy neighbor heuristics
- **Templated design**: Works with any integral index type (`int32_t`, `int64_t`, `uint32_t`, etc.)
- **Header-only library**: Easy integration into existing projects
- **Directed multigraphs**: Supports multiple edges between vertices
- **Multiple subgraph copies**: Find extensions for $m \geq 1$ disjoint pattern occurrences
- **Efficient iterators**: Lazy permutation, combination, and sequence generators
- **Hungarian algorithm integration**: Uses optimal bipartite matching for approximation algorithms
- **Comprehensive testing**: Full Google Test suite with 100+ tests

## Building the Project

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 12+, AppleClang 13+)
- CMake 3.20+
- Git (for fetching Google Test)

### Build Scripts

**Debug build** (with sanitizers for development):
```bash
./scripts/build_debug.sh
```

**Release build** (optimized for performance):
```bash
./scripts/build_release.sh
```

**Build and run tests**:
```bash
./scripts/build_debug.sh
./scripts/run_tests.sh
```

### Manual Build

```bash
# Debug build
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -j$(nproc)

# Release build
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j$(nproc)
```

## Usage

### Command Line Interface

```bash
# Basic syntax
./build/bin/release/subgraphs <input_file> [num_copies] [algorithm] [heuristic]

# Run with exact algorithm (default), 1 copy (default)
./build/bin/release/subgraphs Examples/dokladny1.txt

# Run with exact algorithm, 3 copies
./build/bin/release/subgraphs Examples/dokladny1.txt 3

# Run with approximation algorithm v1
./build/bin/release/subgraphs Examples/approx1.txt 1 approx1

# Run with approximation algorithm v2, degree heuristic (default)
./build/bin/release/subgraphs Examples/approx2.txt 1 approx2 degree

# Run with approximation algorithm v2, different heuristics
./build/bin/release/subgraphs Examples/approx2.txt 2 approx2 directed
./build/bin/release/subgraphs Examples/approx2.txt 1 approx2 histogram
./build/bin/release/subgraphs Examples/approx2.txt 1 approx2 structure
./build/bin/release/subgraphs Examples/approx2.txt 1 approx2 greedy
```

**Arguments:**
- `<input_file>` - Path to graph file (required)
- `[num_copies]` - Number of pattern copies to find (default: 1)
- `[algorithm]` - Algorithm type: `exact`, `approx1`, or `approx2` (default: `exact`)
- `[heuristic]` - For `approx2` only: `degree`, `directed`, `directed_ignore`, `histogram`, `structure`, or `greedy` (default: `degree`)

**Available Heuristics:**
- `degree` - Degree difference heuristic
- `directed` - Directed degree heuristic (considers in/out degrees separately)
- `directed_ignore` - Directed degree ignoring surplus edges
- `histogram` - Neighborhood histogram matching
- `structure` - Structure matching with triangle counting
- `greedy` - Greedy neighbor-based weight assignment

### Input File Format

Graph files contain two adjacency matrices:

```
<pattern_size>
<pattern_adjacency_matrix>
<target_size>
<target_adjacency_matrix>
```

Example (`data/sample_graphs0.txt`):
```
3
0 1 0
2 0 1
1 0 0
4
0 2 0 0
0 0 1 0
1 0 0 1
0 0 0 0
```

- First line: number of vertices in pattern graph (3)
- Next 3 lines: adjacency matrix for pattern (edge weights)
- Next line: number of vertices in target graph (4)
- Last 4 lines: adjacency matrix for target

### Program Output

The program displays:
- **Execution parameters**: Algorithm type, heuristic (if applicable), number of copies
- **Graph information**: Number of vertices and edges in pattern and target graphs
- **Adjacency matrices**: Both pattern and target graphs
- **Extension edges**: List of edges to add to the target graph
- **Modified target graph**: Updated adjacency matrix after adding extension edges
- **Execution time**: Total runtime in milliseconds

### Library Usage

```cpp
#include "algorithms/subgraph_algorithm.h"
#include "algorithms/heuristic.h"
#include "utils/graph_loader.h"
#include "graph/multigraph.h"

using namespace Subgraphs;

// Load graphs from file
auto [pattern, target] = GraphLoader<int64_t>::loadFromFile("graphs.txt");

// Number of disjoint pattern copies to find
int m = 2;

// OPTION 1: Exact algorithm (optimal solution)
auto result_exact = SubgraphAlgorithm<int64_t>::run(m, pattern, target);

// OPTION 2: Approximation algorithm v1
auto result_approx1 = SubgraphAlgorithm<int64_t>::run_approx_v1(m, pattern, target);

// OPTION 3: Approximation algorithm v2 with specific heuristic
auto result_approx2 = SubgraphAlgorithm<int64_t>::run_approx_v2(
    m, pattern, target, HeuristicType::STRUCTURE_MATCHING);

// Process results (same format for all algorithms)
if (!result_exact.empty()) {
    std::cout << "Minimal extension found with "
              << result_exact.size() << " unique edges\n";

    for (const auto& edge : result_exact) {
        std::cout << "Add " << (int)edge.count
                  << " edge(s) from " << edge.source
                  << " to " << edge.destination << "\n";
    }
}
```

### Custom Index Types

```cpp
// Use 32-bit integers for smaller graphs
Multigraph<int32_t> smallPattern(3);
auto results = SubgraphAlgorithm<int32_t>::run(1, pattern, target);

// Use 64-bit integers for large graphs (default)
Multigraph<int64_t> largePattern(100);
auto results = SubgraphAlgorithm<int64_t>::run(1, pattern, target);
```

## Project Structure

```
subgraphs/
├── src/
│   ├── include/
│   │   ├── algorithms/
│   │   │   ├── subgraph_algorithm.h    # Main algorithm interface
│   │   │   ├── subgraph_algorithm.inl  # Algorithm implementations
│   │   │   ├── heuristic.h             # Heuristic interface
│   │   │   └── heuristic.inl           # Heuristic implementations
│   │   ├── graph/
│   │   │   ├── multigraph.h            # Graph representation
│   │   │   ├── permutation_iterator.h  # Permutation generator
│   │   │   ├── combination_iterator.h  # Combination generator
│   │   │   └── sequence_iterator.h     # Sequence generator
│   │   └── utils/
│   │       ├── graph_loader.h          # File I/O
│   │       └── graph_printer.h         # Output formatting
│   └── main.cpp                        # CLI application
├── dependencies/
│   └── hungarian-algorithm-cpp/        # Hungarian algorithm library
├── tests/
│   ├── test_multigraph_gtest.cpp       # Graph tests
│   ├── test_iterators_gtest.cpp        # Iterator tests
│   ├── test_subgraph_algorithm_gtest.cpp
│   ├── test_graph_loader_gtest.cpp
│   └── test_sample_graphs_gtest.cpp    # Integration tests
├── Examples/                           # Example graph files
│   ├── dokladny1.txt                   # Exact algorithm examples
│   ├── dokladny2.txt
│   ├── dokladny3.txt
│   ├── approx1.txt                     # Approx v1 examples
│   ├── approx2.txt                     # Approx v2 examples
│   └── ...
├── data/                               # Sample graph files
├── docs/                               # Documentation
├── scripts/
│   ├── build_debug.sh
│   ├── build_release.sh
│   ├── run_debug.sh
│   ├── run_release.sh
│   └── run_tests.sh
├── .clang-tidy                         # Clang-tidy configuration
├── .clang-format                       # Code formatting configuration
└── CMakeLists.txt
```

## Running Tests

### All Tests
```bash
./scripts/run_tests.sh
```

### Specific Test Suites
```bash
cd build
ctest -R Multigraph --output-on-failure    # Graph tests only
ctest -R Iterator --output-on-failure      # Iterator tests only
ctest -R SubgraphAlgorithm --output-on-failure  # Algorithm tests
ctest -R SampleGraph --output-on-failure   # Integration tests
```

### Test Summary
- **119 unit tests** across 5 test suites
- Type-parameterized tests for multiple index types
- 15-second timeout per test suite
- Comprehensive edge case coverage

## Algorithm Overview

### Exact Algorithm

The exact algorithm finds the optimal solution by:

1. **Enumeration**: Generate all possible mappings of pattern vertices to target vertices using combinations
2. **Validation**: For each mapping, check if edges exist using permutations
3. **Aggregation**: Track required edges across all mappings using max aggregation for multiple copies
4. **Optimization**: Return the mapping requiring the minimum number of additional edges

### Approximation Algorithm v1

A faster heuristic approach that:
- Uses greedy vertex selection based on local matching
- Provides good results for simple patterns
- Significantly faster than the exact algorithm

### Approximation Algorithm v2

An advanced approximation using the Hungarian algorithm for optimal bipartite matching with configurable heuristics:

**Available Heuristics:**

1. **Degree Difference** (`degree`):
   - Compares total vertex degrees between pattern and target
   - Best for: General-purpose matching

2. **Directed Degree** (`directed`):
   - Considers in-degree and out-degree separately
   - Best for: Directed graphs with strong asymmetry

3. **Directed Degree Ignore Surplus** (`directed_ignore`):
   - Like directed degree but ignores excess edges in target
   - Best for: Dense target graphs

4. **Neighbor Histogram** (`histogram`):
   - Compares distributions of neighbor degrees
   - Best for: Graphs with distinctive neighborhood structures

5. **Structure Matching** (`structure`):
   - Analyzes local structure including triangles
   - Best for: Graphs with rich local connectivity

6. **Greedy Neighbor** (`greedy`):
   - Assigns weights based on greedy neighbor matching
   - Best for: Patterns with strong local constraints

### Key Components

- **Multigraph**: Adjacency matrix representation supporting multiple edges
- **Lazy Iterators**: Memory-efficient generators for permutations and combinations
- **Edge Frequency Maps**: Track edge requirements across multiple pattern copies
- **Hungarian Algorithm**: Optimal assignment for approximation algorithms
- **Minimal Extension**: Find the mapping minimizing total edge additions

<!-- ## Compiler Flags

**Debug build** includes:
- Address Sanitizer (`-fsanitize=address`)
- Undefined Behavior Sanitizer (`-fsanitize=undefined`)
- Leak Sanitizer (`-fsanitize=leak`)
- All warnings enabled (`-Wall -Wextra -Wpedantic`)

**Release build** includes:
- Maximum optimization (`-O3`)
- Link-Time Optimization (LTO)
- Native architecture targeting (`-march=native`)

## Code Quality

The project includes configurations for maintaining code quality:

- **Clang-Tidy** (`.clang-tidy`): Balanced set of checks for code analysis
- **Clang-Format** (`.clang-format`): Consistent code formatting
- **Comprehensive documentation**: All heuristics fully documented in code -->

## Performance Considerations

### Algorithm Selection

- **Exact Algorithm**:
  - Time Complexity: $O(k^n \cdot \binom{k}{n})$ where $k$ is target size and $n$ is pattern size
  - Use when: Need optimal solution, small graphs (pattern ≤ 10 vertices, target ≤ 20 vertices)

- **Approximation v1**:
  - Time Complexity: Significantly faster than exact
  - Use when: Need quick results, solution quality is less critical

- **Approximation v2**:
  - Time Complexity: Faster than exact, uses Hungarian algorithm ($O(n^3)$ for each combination)
  - Use when: Need solutions faster than exact algorithm
  - Choose heuristic based on graph structure

<!-- ### General Guidelines

- **Space Complexity**: $O(k^2)$ for adjacency matrices
- **Practical Limits (Exact Algorithm)**:
  - Pattern graphs: up to ~10 vertices
  - Target graphs: up to ~20 vertices
  - Multiple copies: $m \leq 5$ recommended

- **Practical Limits (Approximation Algorithms)**:
  - Can handle much larger graphs
  - Pattern graphs: up to ~50 vertices
  - Target graphs: up to ~100 vertices

### Optimization Tips

- Use `int32_t` or `uint16_t` instead of `int64_t` for reduced memory usage with smaller graphs
- Use release builds for ~10-100x speedup over debug builds
- For exact algorithm on large patterns, consider breaking into smaller components
- Test different heuristics with approx2 to find the best for your graph structure -->

<!-- ## Contributing -->


## License

Published under MIT license.
