"""
Algorithm v.3: Polynomial-time Approximation for Minimal Multigraph Extension

This module implements a greedy, iterative algorithm to find a minimal-cost extension
to a host multigraph G such that the extended graph contains n subgraphs isomorphic
to a pattern multigraph P.

Strategy:
1. Approximate: Find the best available mapping using a linear cost heuristic
2. Filter: Use k-best assignment solver to find unique target vertex sets
3. Install: Calculate true implementation cost and add missing edges
4. Repeat: Process n times
"""

import numpy as np
from scipy.optimize import linear_sum_assignment


def calculate_degrees(adj_matrix):
    """
    Returns a 1D array of vertex degrees for a given adjacency matrix.
    
    For undirected multigraphs, the degree is the sum of edge multiplicities.
    
    Args:
        adj_matrix (np.ndarray): Adjacency matrix (N x N)
        
    Returns:
        np.ndarray: 1D array of vertex degrees
    """
    return np.sum(adj_matrix, axis=1)


def build_cost_matrix(P_degrees, G_degrees, k, N):
    """
    Builds the N x N linear cost matrix M for the assignment problem.
    
    The cost of mapping pattern vertex u to graph vertex v is |deg(p_u) - deg(g_v)|.
    The first k rows correspond to actual pattern vertices.
    Rows k to N-1 are dummy rows with 0 cost (for padding).
    
    Args:
        P_degrees (np.ndarray): Degrees of pattern vertices (size k)
        G_degrees (np.ndarray): Degrees of graph vertices (size N)
        k (int): Number of pattern vertices
        N (int): Number of graph vertices
        
    Returns:
        np.ndarray: Cost matrix (N x N)
    """
    cost_matrix = np.zeros((N, N))
    
    # Calculate costs for the k real pattern vertices
    for u in range(k):
        for v in range(N):
            cost_matrix[u, v] = abs(P_degrees[u] - G_degrees[v])
    
    # Rows k to N-1 are dummy rows with 0 cost
    # (already initialized to zero)
    
    return cost_matrix


def k_best_assignment_solver(cost_matrix, j, k=None):
    """
    Placeholder for a k-best assignment solver (like Murty's algorithm).
    
    This function returns the j-th best solution to the assignment problem.
    For j=1, it uses scipy's linear_sum_assignment.
    For j>1, it would use Murty's algorithm (mocked here for demonstration).
    
    Args:
        cost_matrix (np.ndarray): Cost matrix (N x N)
        j (int): Which best solution to return (1 = best, 2 = 2nd best, etc.)
        k (int, optional): Number of pattern vertices. If None, inferred from cost_matrix.
        
    Returns:
        np.ndarray: Mapping array of size k, where mapping[u] = v means p_u -> g_v
    """
    N = cost_matrix.shape[0]
    
    # Determine k if not provided
    if k is None:
        # Find where dummy rows start (all zeros)
        k = N
        for i in range(N):
            if np.all(cost_matrix[i, :] == 0):
                k = i
                break
    
    if j == 1:
        # Use scipy's linear_sum_assignment for the best solution
        row_ind, col_ind = linear_sum_assignment(cost_matrix)
        
        # Extract only the first k assignments (the real pattern vertices)
        mapping = col_ind[row_ind < k]
        return mapping
    else:
        # This is where Murty's algorithm would generate the j-th solution
        # For demonstration purposes, we implement a simple heuristic:
        # Perturb the best solution by swapping assignments
        
        print(f"Warning: k-best solver for j={j} is not fully implemented. Using heuristic.")
        
        # Get the best solution first
        row_ind, col_ind = linear_sum_assignment(cost_matrix)
        base_mapping = col_ind[row_ind < k]
        
        # Create a perturbation: try to find an alternative valid assignment
        # by selecting different vertices
        # Simple strategy: shift indices cyclically
        available_vertices = set(range(N))
        new_mapping = np.copy(base_mapping)
        
        # Try to swap (j-1) vertices to create a different solution
        swap_count = min(j - 1, k)
        for swap_idx in range(swap_count):
            # Remove current assignment from available set temporarily
            current_vertex = new_mapping[swap_idx]
            remaining = available_vertices - set(new_mapping)
            
            if len(remaining) > 0:
                # Pick a different vertex (preferably with similar cost)
                alternatives = list(remaining)
                costs = [cost_matrix[swap_idx, alt] for alt in alternatives]
                best_alt_idx = np.argmin(costs)
                new_mapping[swap_idx] = alternatives[best_alt_idx]
        
        return new_mapping


def calculate_extension_cost(A_curr, A_P, mapping):
    """
    Calculates the 'real' (quadratic) cost C_ext for a given mapping.
    
    The cost is the total number of edges that need to be added to A_curr
    to ensure the subgraph induced by the mapping is isomorphic to P.
    
    Args:
        A_curr (np.ndarray): Current graph adjacency matrix (N x N)
        A_P (np.ndarray): Pattern adjacency matrix (k x k)
        mapping (np.ndarray): Mapping array (size k)
        
    Returns:
        int: Number of edges to add
    """
    k = A_P.shape[0]
    cost = 0
    
    for u in range(k):
        for v in range(u, k):  # Iterate over upper triangle
            g_u = mapping[u]
            g_v = mapping[v]
            
            w_needed = A_P[u, v]
            w_current = A_curr[g_u, g_v]
            
            w_missing = max(0, w_needed - w_current)
            cost += w_missing
    
    return cost


def apply_extension(A_curr, A_P, mapping):
    """
    Returns a new adjacency matrix with the missing edges added.
    
    For each edge (u, v) in P with multiplicity w, ensure that the corresponding
    edge (g_u, g_v) in A_curr has at least multiplicity w.
    
    Args:
        A_curr (np.ndarray): Current graph adjacency matrix (N x N)
        A_P (np.ndarray): Pattern adjacency matrix (k x k)
        mapping (np.ndarray): Mapping array (size k)
        
    Returns:
        np.ndarray: Updated adjacency matrix with extensions applied
    """
    k = A_P.shape[0]
    A_new = A_curr.copy()
    
    for u in range(k):
        for v in range(u, k):
            g_u = mapping[u]
            g_v = mapping[v]
            
            w_needed = A_P[u, v]
            w_current = A_new[g_u, g_v]
            
            w_missing = max(0, w_needed - w_current)
            
            if w_missing > 0:
                A_new[g_u, g_v] += w_missing
                if g_u != g_v:  # Ensure symmetry for undirected graphs
                    A_new[g_v, g_u] += w_missing
    
    return A_new


def find_minimal_extension_v3(A_G, A_P, n):
    """
    Main function: Find a minimal-cost extension to host graph G such that
    the extended graph contains n isomorphic copies of pattern graph P.
    
    Algorithm:
    1. Initialize working graph and tracking structures
    2. For each of n iterations:
       a. Calculate vertex degrees
       b. Build linear cost matrix based on degree differences
       c. Find unique mapping using k-best assignment solver
       d. Calculate true extension cost
       e. Apply extension to working graph
    3. Return extended graph and total cost
    
    Args:
        A_G (np.ndarray): Host graph adjacency matrix (N x N)
        A_P (np.ndarray): Pattern graph adjacency matrix (k x k)
        n (int): Target number of isomorphic copies
        
    Returns:
        tuple: (A_curr, total_cost)
            - A_curr (np.ndarray): Final extended adjacency matrix
            - total_cost (int): Total number of edges added
    """
    # Initialize
    A_curr = A_G.copy()
    total_cost = 0
    used_vertex_sets = set()
    
    # Get dimensions
    N = A_G.shape[0]
    k = A_P.shape[0]
    
    # Pre-calculate pattern degrees (constant across iterations)
    P_degrees = calculate_degrees(A_P)
    
    print(f"Starting Algorithm v.3: Finding {n} copies of pattern (k={k}) in graph (N={N})")
    print(f"Pattern degrees: {P_degrees}")
    print()
    
    # Main loop: find n unique mappings
    for i in range(n):
        print(f"--- Iteration {i+1}/{n} ---")
        
        # Calculate current graph degrees
        G_degrees = calculate_degrees(A_curr)
        
        # Build cost matrix for assignment problem
        cost_matrix = build_cost_matrix(P_degrees, G_degrees, k, N)
        
        # Inner filtering loop: find a unique mapping
        j = 1
        while True:
            # Get j-th best assignment
            mapping = k_best_assignment_solver(cost_matrix, j, k)
            
            # Extract target vertex set
            target_set = frozenset(mapping)
            
            # Check uniqueness
            if target_set not in used_vertex_sets:
                used_vertex_sets.add(target_set)
                print(f"  Found unique mapping (attempt {j}): {mapping}")
                break
            
            print(f"  Mapping attempt {j} already used: {set(mapping)}")
            j += 1
            
            # Safety check: prevent infinite loops
            if j > 1000:
                print(f"  ERROR: Could not find unique mapping after {j} attempts!")
                print(f"  This may indicate insufficient vertices for {n} unique copies.")
                raise RuntimeError("Unable to find sufficient unique mappings")
        
        # Calculate the real implementation cost
        iteration_cost = calculate_extension_cost(A_curr, A_P, mapping)
        print(f"  Extension cost: {iteration_cost}")
        
        # Apply the extension
        A_curr = apply_extension(A_curr, A_P, mapping)
        
        # Update total cost
        total_cost += iteration_cost
        print(f"  Total cost so far: {total_cost}")
        print()
    
    print(f"Algorithm complete. Total edges added: {total_cost}")
    return A_curr, total_cost


# Example usage and testing
if __name__ == "__main__":
    # Example 1: Simple triangle pattern in a larger graph
    print("=" * 60)
    print("Example 1: Triangle pattern (K3)")
    print("=" * 60)
    
    # Host graph: 6 vertices, sparse connectivity
    N = 6
    A_G = np.array([
        [0, 1, 0, 0, 0, 0],
        [1, 0, 1, 0, 0, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 0, 1, 0, 1],
        [0, 0, 0, 0, 1, 0]
    ])
    
    # Pattern: Triangle (K3)
    A_P = np.array([
        [0, 1, 1],
        [1, 0, 1],
        [1, 1, 0]
    ])
    
    # Find 2 copies
    n = 2
    
    print("\nHost graph adjacency matrix:")
    print(A_G)
    print("\nPattern graph adjacency matrix (Triangle):")
    print(A_P)
    print(f"\nTarget: {n} copies\n")
    
    A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
    
    print("\nFinal extended graph:")
    print(A_result)
    print(f"\nTotal cost: {cost} edges added")
    
    # Example 2: Edge with multiplicity
    print("\n" + "=" * 60)
    print("Example 2: Multi-edge pattern")
    print("=" * 60)
    
    # Host graph: 4 vertices
    A_G2 = np.array([
        [0, 1, 0, 0],
        [1, 0, 0, 0],
        [0, 0, 0, 1],
        [0, 0, 1, 0]
    ])
    
    # Pattern: Two vertices with double edge
    A_P2 = np.array([
        [0, 2],
        [2, 0]
    ])
    
    n2 = 2
    
    print("\nHost graph adjacency matrix:")
    print(A_G2)
    print("\nPattern graph adjacency matrix (Double edge):")
    print(A_P2)
    print(f"\nTarget: {n2} copies\n")
    
    A_result2, cost2 = find_minimal_extension_v3(A_G2, A_P2, n2)
    
    print("\nFinal extended graph:")
    print(A_result2)
    print(f"\nTotal cost: {cost2} edges added")
