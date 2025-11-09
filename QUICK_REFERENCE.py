"""
QUICK REFERENCE GUIDE - Algorithm v.3
======================================

This is a quick reference for using the Minimal Multigraph Extension algorithm.

INSTALLATION
------------
pip install numpy scipy

BASIC USAGE
-----------
from algorithm_v3 import find_minimal_extension_v3
import numpy as np

# Define host graph (N x N adjacency matrix)
A_G = np.array([[0, 1], [1, 0]])

# Define pattern graph (k x k adjacency matrix)  
A_P = np.array([[0, 1], [1, 0]])

# Find n copies
A_result, cost = find_minimal_extension_v3(A_G, A_P, n=1)

GRAPH REPRESENTATION
--------------------
- Use NumPy arrays for adjacency matrices
- A[i,j] = multiplicity of edge between vertex i and j
- A[i,j] = 0 means no edge
- A[i,j] = k means k parallel edges (multigraph)
- For undirected graphs: A[i,j] = A[j,i]

EXAMPLE PATTERNS
----------------

# Edge (2 vertices)
A_P = np.array([[0, 1], [1, 0]])

# Triangle (3 vertices)
A_P = np.array([
    [0, 1, 1],
    [1, 0, 1],
    [1, 1, 0]
])

# Square (4 vertices)
A_P = np.array([
    [0, 1, 0, 1],
    [1, 0, 1, 0],
    [0, 1, 0, 1],
    [1, 0, 1, 0]
])

# Path (4 vertices)
A_P = np.array([
    [0, 1, 0, 0],
    [1, 0, 1, 0],
    [0, 1, 0, 1],
    [0, 0, 1, 0]
])

# Double edge (multigraph)
A_P = np.array([[0, 2], [2, 0]])

# Star (1 center, 3 leaves)
A_P = np.array([
    [0, 1, 1, 1],
    [1, 0, 0, 0],
    [1, 0, 0, 0],
    [1, 0, 0, 0]
])

COMMON USE CASES
----------------

1. FIND IF PATTERN EXISTS (cost = 0)
   A_result, cost = find_minimal_extension_v3(A_G, A_P, n=1)
   if cost == 0:
       print("Pattern already exists in graph")

2. MULTIPLE DISJOINT COPIES
   # Ensure n vertex sets are unique
   A_result, cost = find_minimal_extension_v3(A_G, A_P, n=5)

3. MINIMAL EDGE ADDITIONS
   # Returns the minimum edges needed
   A_result, cost = find_minimal_extension_v3(A_G, A_P, n=1)
   print(f"Need to add {cost} edges")

4. FIND ADDED EDGES
   A_result, cost = find_minimal_extension_v3(A_G, A_P, n=1)
   N = A_G.shape[0]
   for i in range(N):
       for j in range(i+1, N):
           if A_result[i,j] > A_G[i,j]:
               print(f"Add edge: {i} -- {j}")

HELPER FUNCTIONS
----------------

# Calculate vertex degrees
from algorithm_v3 import calculate_degrees
degrees = calculate_degrees(A_G)

# Calculate cost without modifying graph
from algorithm_v3 import calculate_extension_cost
mapping = np.array([0, 1, 2])  # pattern vertices -> graph vertices
cost = calculate_extension_cost(A_G, A_P, mapping)

# Apply extension manually
from algorithm_v3 import apply_extension
A_new = apply_extension(A_G, A_P, mapping)

CONSTRAINTS
-----------
- k (pattern vertices) ≤ N (graph vertices)
- n unique copies requires N ≥ n * k (approximately)
- Undirected graphs only (symmetric matrices)
- Non-negative integer edge multiplicities

COMPLEXITY
----------
Time: O(n × N³)
Space: O(N² + n × k)

COMMON ERRORS
-------------

1. IndexError: index out of bounds
   → Check that k ≤ N

2. RuntimeError: Unable to find sufficient unique mappings
   → Not enough vertices for n unique copies
   → Try reducing n or increasing N

3. Asymmetric result matrix
   → Input should be symmetric for undirected graphs
   → Ensure A_G[i,j] == A_G[j,i]

TESTING
-------
# Run test suite
python src/test_algorithm_v3.py

# Run examples
python src/examples_v3.py

# Custom test
from algorithm_v3 import find_minimal_extension_v3
import numpy as np

A_G = np.eye(5)  # Your graph
A_P = np.ones((2,2)) - np.eye(2)  # Your pattern
A_result, cost = find_minimal_extension_v3(A_G, A_P, n=2)
assert cost >= 0
assert np.array_equal(A_result, A_result.T)  # Symmetric
assert np.all(A_result >= A_G)  # No edges removed

PERFORMANCE TIPS
----------------
1. For large N, consider reducing k (pattern size)
2. Use sparse graphs when possible (fewer initial edges)
3. Limit n to reasonable values (< N/k)
4. Profile with: python -m cProfile src/algorithm_v3.py

VISUALIZATION (Optional)
------------------------
# Using matplotlib/networkx (install separately)
import networkx as nx
import matplotlib.pyplot as plt

G = nx.from_numpy_array(A_result)
nx.draw(G, with_labels=True)
plt.show()

DEBUGGING
---------
# Enable verbose output (already enabled in algorithm)
# Check intermediate states:
A_result, cost = find_minimal_extension_v3(A_G, A_P, n=1)
# Algorithm prints progress automatically

# Verify result
def verify_extension(A_G, A_result, cost):
    # Check cost matches actual additions
    actual_cost = np.sum(A_result - A_G) // 2  # Divide by 2 for undirected
    assert actual_cost == cost
    # Check no edges removed
    assert np.all(A_result >= A_G)
    # Check symmetry
    assert np.array_equal(A_result, A_result.T)

FURTHER READING
---------------
- Full documentation: ALGORITHM_V3_README.md
- Implementation: src/algorithm_v3.py
- Test suite: src/test_algorithm_v3.py
- Examples: src/examples_v3.py

CONTACT & SUPPORT
-----------------
- Review test cases for more examples
- Check examples_v3.py for practical applications
- Read ALGORITHM_V3_README.md for detailed explanation
"""

if __name__ == "__main__":
    print(__doc__)
