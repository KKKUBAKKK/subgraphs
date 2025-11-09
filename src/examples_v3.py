"""
Example usage of Algorithm v.3 for Minimal Multigraph Extension

This script demonstrates practical applications of the algorithm.
"""

import numpy as np
from algorithm_v3 import find_minimal_extension_v3


def example_1_social_network():
    """
    Example 1: Finding triangles (friend groups) in a social network
    
    Scenario: We have a sparse social network and want to suggest connections
    to form at least 3 friend groups (triangles).
    """
    print("=" * 70)
    print("Example 1: Social Network - Finding Friend Triangles")
    print("=" * 70)
    
    # Social network: 8 people, some existing friendships
    print("\nInitial friendships:")
    print("Person 0 - Person 1")
    print("Person 1 - Person 2")
    print("Person 3 - Person 4")
    print("Person 5 - Person 6")
    print("Person 6 - Person 7")
    
    A_G = np.array([
        [0, 1, 0, 0, 0, 0, 0, 0],
        [1, 0, 1, 0, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 0],
        [0, 0, 0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 1, 0],
        [0, 0, 0, 0, 0, 1, 0, 1],
        [0, 0, 0, 0, 0, 0, 1, 0]
    ])
    
    # Pattern: Triangle (3 mutual friends)
    A_P = np.array([
        [0, 1, 1],
        [1, 0, 1],
        [1, 1, 0]
    ])
    
    # Goal: Form 3 friend triangles
    n = 3
    
    print(f"\nGoal: Suggest minimal connections to form {n} friend triangles")
    print("\nRunning algorithm...")
    
    A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
    
    print(f"\n✓ Solution found!")
    print(f"✓ Suggested {cost} new friendships")
    
    # Find which connections were added
    print("\nRecommended friend connections:")
    added_edges = []
    for i in range(8):
        for j in range(i+1, 8):
            if A_result[i, j] > A_G[i, j]:
                added_edges.append((i, j))
                print(f"  • Connect Person {i} with Person {j}")
    
    return A_result, cost


def example_2_road_network():
    """
    Example 2: Building redundant paths in a road network
    
    Scenario: We want to ensure there are multiple alternative routes
    between locations by adding minimal new roads.
    """
    print("\n" + "=" * 70)
    print("Example 2: Road Network - Creating Alternative Routes")
    print("=" * 70)
    
    # Road network: 6 locations
    print("\nExisting roads:")
    print("Location 0 ↔ Location 1")
    print("Location 2 ↔ Location 3")
    print("Location 4 ↔ Location 5")
    
    A_G = np.array([
        [0, 1, 0, 0, 0, 0],
        [1, 0, 0, 0, 0, 0],
        [0, 0, 0, 1, 0, 0],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 0, 1],
        [0, 0, 0, 0, 1, 0]
    ])
    
    # Pattern: Path with 3 segments (ensuring connectivity)
    A_P = np.array([
        [0, 1, 0, 0],
        [1, 0, 1, 0],
        [0, 1, 0, 1],
        [0, 0, 1, 0]
    ])
    
    n = 2
    
    print(f"\nGoal: Build roads to create {n} long paths with 4 connected locations")
    print("\nRunning algorithm...")
    
    A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
    
    print(f"\n✓ Solution found!")
    print(f"✓ Need to build {cost} new road segments")
    
    print("\nNew roads to build:")
    for i in range(6):
        for j in range(i+1, 6):
            if A_result[i, j] > A_G[i, j]:
                print(f"  • Build road: Location {i} ↔ Location {j}")
    
    return A_result, cost


def example_3_network_redundancy():
    """
    Example 3: Adding redundant connections to a computer network
    
    Scenario: Ensure network has redundant multi-connections between
    critical nodes for fault tolerance.
    """
    print("\n" + "=" * 70)
    print("Example 3: Computer Network - Adding Redundant Connections")
    print("=" * 70)
    
    # Network: 5 servers with single connections
    print("\nCurrent network connections:")
    print("Server 0 → Server 1 (1 cable)")
    print("Server 1 → Server 2 (1 cable)")
    print("Server 3 → Server 4 (1 cable)")
    
    A_G = np.array([
        [0, 1, 0, 0, 0],
        [1, 0, 1, 0, 0],
        [0, 1, 0, 0, 0],
        [0, 0, 0, 0, 1],
        [0, 0, 0, 1, 0]
    ])
    
    # Pattern: Triple-redundant connection (3 cables between two servers)
    A_P = np.array([
        [0, 3],
        [3, 0]
    ])
    
    n = 2
    
    print(f"\nGoal: Add cables to create {n} triple-redundant links")
    print("\nRunning algorithm...")
    
    A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
    
    print(f"\n✓ Solution found!")
    print(f"✓ Need to add {cost} cables")
    
    print("\nCables to add:")
    for i in range(5):
        for j in range(i+1, 5):
            added = A_result[i, j] - A_G[i, j]
            if added > 0:
                print(f"  • Add {added} cable(s): Server {i} ↔ Server {j}")
    
    return A_result, cost


def example_4_chemical_structure():
    """
    Example 4: Finding cyclic structures in a molecular graph
    
    Scenario: Identify and suggest bonds to form specific ring structures.
    """
    print("\n" + "=" * 70)
    print("Example 4: Molecular Chemistry - Forming Ring Structures")
    print("=" * 70)
    
    # Molecular graph: atoms with some bonds
    print("\nExisting atomic bonds:")
    print("Atom 0 = Atom 1")
    print("Atom 1 = Atom 2")
    print("Atom 3 = Atom 4")
    
    A_G = np.array([
        [0, 2, 0, 0, 0, 0],
        [2, 0, 1, 0, 0, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 0, 1, 0, 0],
        [0, 0, 0, 0, 0, 0]
    ])
    
    # Pattern: Benzene-like hexagonal ring (simplified)
    # Using a square for simplicity
    A_P = np.array([
        [0, 1, 0, 1],
        [1, 0, 1, 0],
        [0, 1, 0, 1],
        [1, 0, 1, 0]
    ])
    
    n = 1
    
    print(f"\nGoal: Form {n} square ring structure")
    print("\nRunning algorithm...")
    
    A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
    
    print(f"\n✓ Solution found!")
    print(f"✓ Need to add {cost} bonds")
    
    print("\nBonds to add:")
    for i in range(6):
        for j in range(i+1, 6):
            added = A_result[i, j] - A_G[i, j]
            if added > 0:
                bond_type = "single" if added == 1 else "double" if added == 2 else "triple"
                print(f"  • Add {bond_type} bond: Atom {i} — Atom {j}")
    
    return A_result, cost


def example_5_custom():
    """
    Example 5: Custom graph - user can modify this
    """
    print("\n" + "=" * 70)
    print("Example 5: Custom Graph Extension")
    print("=" * 70)
    
    # Define your own host graph here
    A_G = np.array([
        [0, 1, 1, 0],
        [1, 0, 0, 1],
        [1, 0, 0, 1],
        [0, 1, 1, 0]
    ])
    
    # Define your own pattern here
    A_P = np.array([
        [0, 2, 1],
        [2, 0, 1],
        [1, 1, 0]
    ])
    
    n = 1
    
    print("\nHost graph adjacency matrix:")
    print(A_G)
    print("\nPattern graph adjacency matrix:")
    print(A_P)
    print(f"\nFinding {n} copies...")
    
    A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
    
    print(f"\nTotal cost: {cost} edges")
    print("\nFinal extended graph:")
    print(A_result)
    
    return A_result, cost


if __name__ == "__main__":
    # Run all examples
    print("\n")
    print("╔" + "═" * 68 + "╗")
    print("║" + " " * 15 + "ALGORITHM v.3 - PRACTICAL EXAMPLES" + " " * 19 + "║")
    print("╚" + "═" * 68 + "╝")
    
    examples = [
        example_1_social_network,
        example_2_road_network,
        example_3_network_redundancy,
        example_4_chemical_structure,
        example_5_custom
    ]
    
    results = []
    for example_func in examples:
        try:
            result = example_func()
            results.append(result)
            print("\n" + "─" * 70)
        except Exception as e:
            print(f"\n⚠️  Error in {example_func.__name__}: {e}")
            print("─" * 70)
    
    print("\n")
    print("╔" + "═" * 68 + "╗")
    print("║" + " " * 25 + "ALL EXAMPLES COMPLETE!" + " " * 22 + "║")
    print("╚" + "═" * 68 + "╝")
