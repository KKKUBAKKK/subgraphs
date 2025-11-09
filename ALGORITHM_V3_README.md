# Algorithm v.3: Minimal Multigraph Extension

## Overview

This implementation provides a polynomial-time approximation algorithm for the **Minimal Multigraph Extension** problem. The goal is to find a minimal-cost extension to a host multigraph G such that the extended graph contains n subgraphs isomorphic to a pattern multigraph P.

## Problem Definition

**Input:**
- Host multigraph: G = (V_G, E_G) with N vertices
- Pattern multigraph: P = (V_P, E_P) with k vertices (k ≤ N)
- Target number: n (number of isomorphic copies to find)

**Output:**
- Extended adjacency matrix A_curr
- Total cost (number of edges added)

**Constraints:**
- Each of the n subgraphs must be "unique" (use distinct vertex sets)
- Graphs are represented by adjacency matrices where A[i,j] stores edge multiplicity
- Extension means adding edges to G (never removing)

## Algorithm Strategy

Algorithm v.3 is a **greedy, iterative** approach that processes each of the n copies sequentially:

### For each iteration i ∈ {1, ..., n}:

1. **Approximate:** Find the best available mapping using a linear cost heuristic (C_linear) based on vertex degree differences. This is formulated as an Assignment Problem.

2. **Filter:** Use a k-best assignment solver (e.g., Murty's algorithm) to generate candidate mappings. Keep fetching the "next-best" mapping (j=1,2,3,...) until we find one whose k-element target vertex set has not been used in previous iterations.

3. **Install:** Once a new, unique mapping f_i is found, calculate its true implementation cost (C_ext, the "quadratic" cost) and add the missing edges to our working graph G_curr.

4. **Repeat:** Continue until n unique mappings are found.

## Implementation Details

### Main Function

```python
find_minimal_extension_v3(A_G, A_P, n)
```

**Parameters:**
- `A_G`: numpy array (N×N) - Host graph adjacency matrix
- `A_P`: numpy array (k×k) - Pattern graph adjacency matrix  
- `n`: int - Target number of isomorphic copies

**Returns:**
- `A_curr`: numpy array (N×N) - Final extended adjacency matrix
- `total_cost`: int - Total number of edges added

### Helper Functions

#### 1. `calculate_degrees(adj_matrix)`
Computes the degree of each vertex (sum of edge multiplicities).

#### 2. `build_cost_matrix(P_degrees, G_degrees, k, N)`
Builds the N×N linear cost matrix for the assignment problem.
- Cost of mapping pattern vertex u to graph vertex v: |deg(p_u) - deg(g_v)|
- First k rows: actual pattern vertices
- Rows k to N-1: dummy rows (zero cost)

#### 3. `k_best_assignment_solver(cost_matrix, j)`
Returns the j-th best solution to the assignment problem.
- For j=1: uses `scipy.optimize.linear_sum_assignment`
- For j>1: would use Murty's algorithm (mocked with heuristic in current implementation)

#### 4. `calculate_extension_cost(A_curr, A_P, mapping)`
Calculates the true "quadratic" cost C_ext for a given mapping.
Returns the number of edges needed to create the isomorphic copy.

#### 5. `apply_extension(A_curr, A_P, mapping)`
Adds the missing edges to A_curr to implement the mapping.
Ensures symmetry for undirected graphs.

## Usage

### Basic Usage

```python
import numpy as np
from algorithm_v3 import find_minimal_extension_v3

# Define host graph (6 vertices, sparse)
A_G = np.array([
    [0, 1, 0, 0, 0, 0],
    [1, 0, 1, 0, 0, 0],
    [0, 1, 0, 0, 0, 0],
    [0, 0, 0, 0, 1, 0],
    [0, 0, 0, 1, 0, 1],
    [0, 0, 0, 0, 1, 0]
])

# Define pattern (triangle)
A_P = np.array([
    [0, 1, 1],
    [1, 0, 1],
    [1, 1, 0]
])

# Find 2 copies
n = 2
A_result, cost = find_minimal_extension_v3(A_G, A_P, n)

print(f"Total edges added: {cost}")
print(f"Extended graph:\n{A_result}")
```

### Running from Terminal

```bash
# Activate virtual environment
source venv/bin/activate

# Run the implementation with examples
python src/algorithm_v3.py
```

## Dependencies

- Python 3.7+
- NumPy
- SciPy

Install dependencies:
```bash
pip install numpy scipy
```

## Algorithm Complexity

### Time Complexity
For each of the n iterations:
- Building cost matrix: O(kN)
- Assignment problem (Hungarian algorithm): O(N³)
- Extension cost calculation: O(k²)
- Applying extension: O(k²)

**Total:** O(n × N³) for n iterations

In practice, if the k-best solver is efficient and few iterations are needed to find unique mappings, the complexity is manageable for moderate-sized graphs.

### Space Complexity
- O(N²) for storing adjacency matrices
- O(n × k) for tracking used vertex sets

## Key Design Decisions

1. **Linear vs Quadratic Cost:**
   - Linear cost (degree difference) is used for initial approximation
   - Quadratic cost (actual edge count) is used for final implementation
   - This two-phase approach balances efficiency and accuracy

2. **Uniqueness Constraint:**
   - Uses `frozenset` to efficiently track used vertex sets
   - Prevents duplicate mappings by checking before accepting any solution

3. **Symmetry Handling:**
   - Ensures adjacency matrix remains symmetric for undirected graphs
   - Updates both A[i,j] and A[j,i] when adding edges

4. **Multigraph Support:**
   - Adjacency matrix entries represent edge multiplicities
   - Correctly handles edges with multiplicity > 1

## Limitations & Future Work

1. **K-best Assignment Solver:**
   - Current implementation uses a simple heuristic for j>1
   - Full Murty's algorithm would improve solution quality
   - Consider using specialized libraries (e.g., `murty-assignment`)

2. **Greedy Nature:**
   - Algorithm makes locally optimal choices
   - May not find global minimum cost
   - Could be improved with backtracking or branch-and-bound

3. **Scalability:**
   - Cubic complexity limits application to moderate-sized graphs
   - Could explore approximations for the assignment problem

4. **Directed Graphs:**
   - Current implementation assumes undirected graphs
   - Extension to directed graphs requires removing symmetry constraint

## Examples Included

### Example 1: Triangle Pattern
- Host: 6 vertices, sparse connectivity
- Pattern: Complete triangle (K3)
- Target: 2 copies
- Result: 3 edges added

### Example 2: Multi-edge Pattern
- Host: 4 vertices, simple edges
- Pattern: Double edge between two vertices
- Target: 2 copies
- Result: 3 edges added

## References

- Hungarian Algorithm: O(n³) solution for assignment problem
- Murty's Algorithm: K-best solutions for linear assignment
- Graph Isomorphism: NP-intermediate complexity

## License

This implementation is provided for educational and research purposes.

## Author

Algorithmic implementation based on Algorithm v.3 specification for the Minimal Multigraph Extension problem.
