import numpy as np
import sys
sys.path.append('src')
from algorithm_v3_directed import calculate_extension_cost_directed

# Example 1 debug
A_G = np.array([
    [0, 1, 0, 0],
    [0, 0, 1, 0],
    [0, 0, 0, 0],
    [0, 0, 0, 0]
])

A_P = np.array([
    [0, 1],
    [0, 0]
])

print("Example 1 Debug:")
print("Graph G:")
print(A_G)
print("\nPattern P:")
print(A_P)

# Test mapping [0, 1] (should be 0 cost)
mapping1 = np.array([0, 1])
print(f"\nMapping {mapping1}:")
print(f"  Pattern 0 -> Graph 0")
print(f"  Pattern 1 -> Graph 1")
print(f"  Need edge: Graph[0,1] >= Pattern[0,1] = {A_P[0,1]}")
print(f"  Current: Graph[0,1] = {A_G[0,1]}")
cost1 = calculate_extension_cost_directed(A_G, A_P, mapping1)
print(f"  Cost: {cost1}")

# Test mapping [1, 2] (should be 0 cost)
mapping2 = np.array([1, 2])
print(f"\nMapping {mapping2}:")
print(f"  Pattern 0 -> Graph 1")
print(f"  Pattern 1 -> Graph 2")
print(f"  Need edge: Graph[1,2] >= Pattern[0,1] = {A_P[0,1]}")
print(f"  Current: Graph[1,2] = {A_G[1,2]}")
cost2 = calculate_extension_cost_directed(A_G, A_P, mapping2)
print(f"  Cost: {cost2}")

# Test mapping [0, 2] (needs edge 0->2)
mapping3 = np.array([0, 2])
print(f"\nMapping {mapping3}:")
print(f"  Pattern 0 -> Graph 0")
print(f"  Pattern 1 -> Graph 2")
print(f"  Need edge: Graph[0,2] >= Pattern[0,1] = {A_P[0,1]}")
print(f"  Current: Graph[0,2] = {A_G[0,2]}")
cost3 = calculate_extension_cost_directed(A_G, A_P, mapping3)
print(f"  Cost: {cost3}")
