# subgraphs
Algorithm for checking if a graph $G_s$ is a subgraph of $G_b$ in $O(k^n)$. If it isn't, then program finds minimal extension of graph $G_b$ that makes $G_s$ its subgraph also in $O(k^n)$.
There is also a version of this algorithm that uses approximation, to get the final results in $O(n^j)$ time.

# TODO:
1. Decide on graph representation
2. Decide on the metric to determine how much do the graphs resemble each other
3. Checking if $G_s$ is a subgraph of $G_b$
4. Finding minimal extension of $G_b$ so that $G_s$ will be a subgraph of the extended graph
5. Approximation for points 3 and 4

## Project structure (just a suggestion):
```
subgraphs/
├── CMakeLists.txt                 # Root CMake configuration
├── .vscode/
│   ├── settings.json              # VS Code workspace settings
│   ├── launch.json                # Debug configurations
│   ├── tasks.json                 # Build tasks
│   └── extensions.json            # Recommended extensions
├── .clang-format                  # Code formatting rules
├── .clang-tidy                    # Static analysis rules
├── build/                         # Build output (generated)
│   ├── Debug/
│   └── Release/
├── src/
│   ├── CMakeLists.txt             # Source CMake configuration
│   ├── main.cpp                   # Entry point
│   ├── include/                   # Public headers
│   │   ├── config.hpp             # Project configuration
│   │   ├── graph/
│   │   │   ├── graph.hpp          # Base graph class
│   │   │   └── multigraph.hpp     # Directed multigraph class
│   │   ├── algorithms/
│   │   │   ├── exact_algorithm.hpp
│   │   │   ├── approximation_v1.hpp
│   │   │   └── approximation_v2.hpp
│   │   └── utils/
│   │       ├── io.hpp             # File I/O utilities
│   │       └── validation.hpp     # Graph validation
│   ├── graph/                     # Graph implementations
│   │   ├── graph.cpp
│   │   └── multigraph.cpp
│   ├── algorithms/                # Algorithm implementations
│   │   ├── exact_algorithm.cpp
│   │   ├── approximation_v1.cpp
│   │   └── approximation_v2.cpp
│   └── utils/                     # Utility implementations
│       ├── io.cpp
│       └── validation.cpp
├── tests/                         # Unit tests
│   ├── CMakeLists.txt
│   ├── test_graph.cpp
│   ├── test_multigraph.cpp
│   ├── test_exact_algorithm.cpp
│   ├── test_approximation_v1.cpp
│   ├── test_approximation_v2.cpp
│   └── test_validation.cpp
├── data/                          # Test data and examples
│   ├── examples/
│   │   ├── graph_1.txt
│   │   ├── graph_2.txt
│   │   ├── pattern_1.txt
│   │   └── pattern_2.txt
│   └── expected_results/
│       ├── result_1.txt
│       └── result_2.txt
├── docs/                          # Documentation
│   ├── README.md                  # Project overview
│   ├── ALGORITHM.md               # Algorithm description
│   ├── BUILD.md                   # Build instructions
│   ├── ARCHITECTURE.md            # Project architecture
│   └── API.md                     # API documentation
├── scripts/                       # Utility scripts
│   ├── build.sh                   # Build script
│   ├── format.sh                  # Code formatting script
│   ├── analyze.sh                 # Static analysis script
│   └── test.sh                    # Test running script
├── third_party/                   # External dependencies
│   └── (if needed)
├── .gitignore                     # Git ignore rules
├── README.md                      # Project readme
└── LICENSE                        # License file
```
