# Subgraph Extension Algorithm

A C++20 templated library for finding minimal extensions of directed multigraphs. Given a pattern graph $P$ and a target graph $G$, the algorithm determines the minimum set of edges that must be added to $G$ so that it contains $m$ disjoint copies of $P$ as subgraphs.

## Problem Definition

**Input:**
- Pattern graph $P$ with $n$ vertices
- Target graph $G$ with $k$ vertices (where $k \geq n$)
- Number of copies $m$ to find

**Output:**
- Minimal set of edges to add to $G$ such that it contains at least $m$ vertex-disjoint copies of $P$

**Complexity:** $O(k^n \cdot \binom{k}{n})$ for exhaustive search over all possible vertex mappings

## Features

- **Templated design**: Works with any integral index type (`int32_t`, `int64_t`, `uint32_t`, etc.)
- **Header-only library**: Easy integration into existing projects
- **Directed multigraphs**: Supports multiple edges between vertices
- **Multiple subgraph copies**: Find extensions for $m \geq 1$ disjoint pattern occurrences
- **Efficient iterators**: Lazy permutation, combination, and sequence generators
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
# Run with debug build
./scripts/run_debug.sh <graph_file> <num_copies>

# Run with release build
./scripts/run_release.sh <graph_file> <num_copies>

# Example
./scripts/run_release.sh data/sample_graphs0.txt 1
```

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

### Library Usage

```cpp
#include "algorithms/subgraph_algorithm.h"
#include "utils/graph_loader.h"
#include "graph/multigraph.h"

using namespace Subgraphs;

// Load graphs from file
auto [pattern, target] = GraphLoader<int64_t>::loadFromFile("graphs.txt");

// Find minimal extension for m copies
int m = 2;  // Number of disjoint pattern copies to find
auto results = SubgraphAlgorithm<int64_t>::run(m, pattern, target);

// Process results
if (!results.empty()) {
    for (const auto& extensions : results) {
        for (const auto& extension : extensions) {
            std::cout << "Minimal extension found with "
                      << extension.size() << " unique edges\n";

            for (const auto& edge : extension) {
                std::cout << "Add " << (int)edge.count
                         << " edge(s) from " << edge.source
                         << " to " << edge.destination << "\n";
            }
        }
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
│   │   │   └── subgraph_algorithm.h    # Main algorithm
│   │   ├── graph/
│   │   │   ├── multigraph.h            # Graph representation
│   │   │   ├── permutation_iterator.h  # Permutation generator
│   │   │   ├── combination_iterator.h  # Combination generator
│   │   │   └── sequence_iterator.h     # Sequence generator
│   │   └── utils/
│   │       ├── graph_loader.h          # File I/O
│   │       └── graph_printer.h         # Output formatting
│   └── main.cpp                        # CLI application
├── tests/
│   ├── test_multigraph_gtest.cpp       # Graph tests
│   ├── test_iterators_gtest.cpp        # Iterator tests
│   ├── test_subgraph_algorithm_gtest.cpp
│   ├── test_graph_loader_gtest.cpp
│   └── test_sample_graphs_gtest.cpp    # Integration tests
├── data/                               # Sample graph files
├── scripts/
│   ├── build_debug.sh
│   ├── build_release.sh
│   ├── run_debug.sh
│   ├── run_release.sh
│   └── run_tests.sh
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

The algorithm works by:

1. **Enumeration**: Generate all possible mappings of pattern vertices to target vertices using combinations
2. **Validation**: For each mapping, check if edges exist using permutations
3. **Aggregation**: Track required edges across all mappings using max aggregation for multiple copies
4. **Optimization**: Return the mapping requiring the minimum number of additional edges

### Key Components

- **Multigraph**: Adjacency matrix representation supporting multiple edges
- **Lazy Iterators**: Memory-efficient generators for permutations and combinations
- **Edge Frequency Maps**: Track edge requirements across multiple pattern copies
- **Minimal Extension**: Find the mapping minimizing total edge additions

## Compiler Flags

**Debug build** includes:
- Address Sanitizer (`-fsanitize=address`)
- Undefined Behavior Sanitizer (`-fsanitize=undefined`)
- Leak Sanitizer (`-fsanitize=leak`)
- All warnings enabled (`-Wall -Wextra -Wpedantic`)

**Release build** includes:
- Maximum optimization (`-O3`)
- Link-Time Optimization (LTO)
- Native architecture targeting (`-march=native`)

## Performance Considerations

- **Time Complexity**: $O(k^n \cdot \binom{k}{n})$ where $k$ is target size and $n$ is pattern size
- **Space Complexity**: $O(k^2)$ for adjacency matrices
- **Practical Limits**:
  - Pattern graphs: up to ~10 vertices
  - Target graphs: up to ~20 vertices
  - Multiple copies: $m \leq 5$ recommended

For larger graphs, consider:
- Using `int32_t` instead of `int64_t` for reduced memory
- Release builds for ~10-100x speedup
- Breaking patterns into smaller components

<!-- ## Contributing -->


## License

Published under MIT license.
