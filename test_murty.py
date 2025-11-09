"""
Test Murty's algorithm implementation for k-best assignments
"""
import numpy as np
import sys
sys.path.append('src')
from algorithm_v3_directed import KBestAssignments, k_best_assignment_solver

print("=" * 70)
print("Testing Murty's Algorithm for K-Best Assignment")
print("=" * 70)

# Test 1: Simple 4x4 cost matrix
print("\nTest 1: Simple 4x4 matrix")
cost_matrix = np.array([
    [1, 2, 3, 4],
    [2, 4, 6, 8],
    [3, 6, 9, 12],
    [0, 0, 0, 0]  # Dummy row
])

k = 3
solver = KBestAssignments(cost_matrix, k)

print(f"Cost matrix:\n{cost_matrix}")
print(f"\nFirst 5 best solutions:")
for i in range(min(5, len(solver.solutions))):
    cost, mapping = solver.solutions[i]
    print(f"  Solution {i+1}: mapping={mapping}, cost={cost}")

# Test 2: Using k_best_assignment_solver
print("\n" + "=" * 70)
print("Test 2: Using k_best_assignment_solver function")
print("=" * 70)

cost_matrix_2 = np.array([
    [0, 1, 2],
    [1, 0, 1],
    [0, 0, 0]  # Dummy row
])

k = 2
print(f"\nCost matrix:\n{cost_matrix_2}")
print(f"\nGetting solutions 1-5:")
for j in range(1, 6):
    mapping = k_best_assignment_solver(cost_matrix_2, j, k)
    print(f"  j={j}: mapping={mapping}")

# Test 3: Real example - finding second-best when first is taken
print("\n" + "=" * 70)
print("Test 3: Iterative selection (simulating used vertex sets)")
print("=" * 70)

A_G = np.array([
    [0, 1, 0, 0],
    [0, 0, 1, 0],
    [0, 0, 0, 1],
    [0, 0, 0, 0]
])

# Simple pattern: single edge
A_P = np.array([
    [0, 1],
    [0, 0]
])

# Build cost matrix (simplified - just use zeros)
cost_matrix_3 = np.zeros((4, 4))
k = 2

print("Finding 3 unique mappings:")
used = set()
for iteration in range(1, 4):
    print(f"\nIteration {iteration}:")
    j = 1
    while True:
        mapping = k_best_assignment_solver(cost_matrix_3, j, k)
        vertex_set = frozenset(mapping)
        
        if vertex_set not in used:
            used.add(vertex_set)
            print(f"  Found unique mapping (attempt {j}): {mapping}")
            break
        else:
            print(f"  Mapping {mapping} already used, trying next...")
            j += 1
            
        if j > 10:
            print("  ERROR: Too many attempts!")
            break

print("\n" + "=" * 70)
print("All tests completed!")
print("=" * 70)
