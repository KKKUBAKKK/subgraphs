"""
Algorithm v.3 for DIRECTED Multigraphs

Modified version that handles directed multigraphs where:
- G[i,j] represents the number of directed edges from vertex i to vertex j
- G[i,j] ≠ G[j,i] in general
- Self-loops are allowed
"""

import numpy as np
from scipy.optimize import linear_sum_assignment


def calculate_out_degrees(adj_matrix):
    """Returns out-degrees (sum of outgoing edges for each vertex)"""
    return np.sum(adj_matrix, axis=1)


def calculate_in_degrees(adj_matrix):
    """Returns in-degrees (sum of incoming edges for each vertex)"""
    return np.sum(adj_matrix, axis=0)


def calculate_total_degrees(adj_matrix):
    """Returns total degrees (in + out) for each vertex"""
    return calculate_out_degrees(adj_matrix) + calculate_in_degrees(adj_matrix)


def build_cost_matrix_quadratic_directed(A_curr, A_P, k, N, used_sets):
    """
    Builds cost matrix using TRUE quadratic cost for each possible mapping.
    This is more expensive (O(N^k)) but gives optimal assignment.
    
    For small k (k <= 4), this is feasible. For larger k, use linear approximation.
    """
    from itertools import combinations
    
    cost_matrix = np.zeros((N, N))
    
    # For each possible assignment of k pattern vertices to N graph vertices
    # we need to calculate the true cost
    # But assignment problem needs cost[u,v] = cost of assigning pattern vertex u to graph vertex v
    
    # Strategy: For each (u, v) pair, estimate the cost assuming u->v
    # by looking at best possible assignments for other pattern vertices
    for u in range(k):
        for v in range(N):
            # Estimate: if we fix u->v, what's the minimum cost for the rest?
            # Simplified: just count missing edges from v to any vertex for u's neighbors
            cost = 0
            
            # Check outgoing edges from u
            for u2 in range(k):
                if A_P[u, u2] > 0:
                    # Need edge v -> some_vertex with multiplicity A_P[u, u2]
                    # Find best target (vertex with most existing edges from v)
                    best_existing = 0
                    for v2 in range(N):
                        if v2 != v:  # Can't map to same vertex
                            best_existing = max(best_existing, A_curr[v, v2])
                    cost += max(0, A_P[u, u2] - best_existing)
            
            # Check incoming edges to u
            for u2 in range(k):
                if A_P[u2, u] > 0:
                    # Need edge some_vertex -> v with multiplicity A_P[u2, u]
                    best_existing = 0
                    for v2 in range(N):
                        if v2 != v:
                            best_existing = max(best_existing, A_curr[v2, v])
                    cost += max(0, A_P[u2, u] - best_existing)
            
            cost_matrix[u, v] = cost
    
    # Pad with dummy rows (all zeros)
    # Rows k to N-1 are dummy rows
    
    return cost_matrix


def build_cost_matrix_directed(P_degrees, G_degrees, k, N):
    """
    Builds the N x N linear cost matrix for directed graphs.
    Uses total degree (in + out) as the heuristic.
    """
    cost_matrix = np.zeros((N, N))
    
    for u in range(k):
        for v in range(N):
            cost_matrix[u, v] = abs(P_degrees[u] - G_degrees[v])
    
    return cost_matrix


class KBestAssignments:
    """
    Implementation of Murty's algorithm for finding k-best assignments.
    
    Murty's algorithm finds the k lowest cost solutions to the linear assignment problem.
    It uses a partitioning scheme with a tree structure where each node represents
    a constrained assignment problem.
    """
    
    def __init__(self, cost_matrix, k_pattern):
        """
        Args:
            cost_matrix: N x N cost matrix
            k_pattern: Number of pattern vertices (only first k_pattern rows matter)
        """
        self.cost_matrix = cost_matrix.copy()
        self.N = cost_matrix.shape[0]
        self.k_pattern = k_pattern
        self.solutions = []
        # self.compute_k_best()
    
    def solve_with_constraints(self, include_constraints, exclude_constraints):
        """
        Solve assignment problem with constraints.
        
        Args:
            include_constraints: list of (row, col) tuples that MUST be in solution
            exclude_constraints: list of (row, col) tuples that MUST NOT be in solution
            
        Returns:
            (cost, assignment) or (None, None) if infeasible
        """
        # Create modified cost matrix
        modified_cost = self.cost_matrix.copy()
        
        # Set excluded assignments to very high cost
        for row, col in exclude_constraints:
            modified_cost[row, col] = 1e10
        
        # For included constraints, we need to ensure they're selected
        # We'll do this by setting their cost to very negative
        # and all other assignments in those rows to very high
        for row, col in include_constraints:
            modified_cost[row, :] = 1e10
            modified_cost[row, col] = -1e10
        
        try:
            row_ind, col_ind = linear_sum_assignment(modified_cost)
            
            # Verify all include constraints are satisfied
            assignment = list(zip(row_ind, col_ind))
            for constraint in include_constraints:
                if constraint not in assignment:
                    return None, None
            
            # Calculate true cost (without modifications)
            true_cost = sum(self.cost_matrix[r, c] for r, c in assignment)
            
            # Extract only first k_pattern assignments
            mapping = col_ind[row_ind < self.k_pattern]
            
            return true_cost, mapping
            
        except ValueError:
            return None, None
    
    def compute_k_best(self, max_solutions=100):
        """
        Compute k-best solutions using Murty's algorithm.
        
        Args:
            max_solutions: Maximum number of solutions to generate
        """
        import heapq
        
        # Start with unconstrained problem
        cost, mapping = self.solve_with_constraints([], [])
        if cost is None:
            return
        
        # Priority queue: (cost, include_constraints, exclude_constraints, mapping)
        queue = [(cost, [], [], mapping)]
        
        # Track seen mappings to avoid duplicates
        seen_mappings = set()
        
        while queue and len(self.solutions) < max_solutions:
            current_cost, includes, excludes, current_mapping = heapq.heappop(queue)
            
            # Check if we've seen this mapping before
            mapping_tuple = tuple(current_mapping)
            if mapping_tuple in seen_mappings:
                continue
            
            seen_mappings.add(mapping_tuple)
            
            # Add this solution
            self.solutions.append((current_cost, current_mapping))
            
            # Create partitions by excluding each assignment one by one
            # Only partition on the first k_pattern assignments
            for i in range(self.k_pattern):
                row = i
                col = current_mapping[i]
                
                # Create new partition with this assignment excluded
                new_excludes = excludes + [(row, col)]
                
                # Try to solve with this new constraint
                new_cost, new_mapping = self.solve_with_constraints(includes, new_excludes)
                
                if new_cost is not None:
                    heapq.heappush(queue, (new_cost, includes, new_excludes, new_mapping))
                
                # For next partition, include this assignment
                includes = includes + [(row, col)]
    
    def get_solution(self, j):
        """
        Get the j-th best solution (1-indexed).
        
        Args:
            j: Solution rank (1 = best, 2 = second best, etc.)
            
        Returns:
            mapping array or None if not enough solutions
        """
        if j <= len(self.solutions):
            return self.solutions[j - 1][1]
        return None


def k_best_assignment_solver(cost_matrix, j, k=None):
    """
    K-best assignment solver using Murty's algorithm.
    
    Args:
        cost_matrix: N x N cost matrix
        j: Which solution to return (1 = best, 2 = second best, etc.)
        k: Number of pattern vertices (if None, inferred from cost matrix)
        
    Returns:
        mapping array of size k
    """
    N = cost_matrix.shape[0]
    
    if k is None:
        k = N
        for i in range(N):
            if np.all(cost_matrix[i, :] == 0):
                k = i
                break
    
    # For j=1, just use standard assignment (faster)
    if j == 1:
        row_ind, col_ind = linear_sum_assignment(cost_matrix)
        mapping = col_ind[row_ind < k]
        return mapping
    
    # For j>1, use Murty's algorithm
    # Only compute up to j solutions (no need to compute all 100)
    solver = KBestAssignments(cost_matrix, k)
    solver.compute_k_best(max_solutions=min(j + 10, 100))  # Compute a few extra for safety
    
    mapping = solver.get_solution(j)
    
    if mapping is None:
        # Fallback: not enough distinct solutions found
        # This can happen when there aren't enough vertices for j unique mappings
        print(f"Warning: Could not find {j}-th best solution. Only {len(solver.solutions)} solutions exist.")
        if len(solver.solutions) > 0:
            # Return the last available solution
            return solver.solutions[-1][1]
        else:
            # Last resort: return arbitrary mapping
            return np.arange(k)
    
    return mapping


def calculate_extension_cost_directed(A_curr, A_P, mapping):
    """
    Calculates extension cost for DIRECTED multigraphs.
    Must check ALL pairs (u,v), not just upper triangle.
    """
    k = A_P.shape[0]
    cost = 0
    
    for u in range(k):
        for v in range(k):  # ALL pairs, not just upper triangle
            g_u = mapping[u]
            g_v = mapping[v]
            
            w_needed = A_P[u, v]
            w_current = A_curr[g_u, g_v]
            
            w_missing = max(0, w_needed - w_current)
            cost += w_missing
    
    return cost


def apply_extension_directed(A_curr, A_P, mapping):
    """
    Applies extension for DIRECTED multigraphs.
    Does NOT enforce symmetry.
    """
    k = A_P.shape[0]
    A_new = A_curr.copy()
    
    for u in range(k):
        for v in range(k):  # ALL pairs
            g_u = mapping[u]
            g_v = mapping[v]
            
            w_needed = A_P[u, v]
            w_current = A_new[g_u, g_v]
            
            w_missing = max(0, w_needed - w_current)
            
            if w_missing > 0:
                A_new[g_u, g_v] += w_missing
                # NO symmetry enforcement for directed graphs
    
    return A_new


def find_minimal_extension_v3_directed(A_G, A_P, n):
    """
    Main function for DIRECTED multigraph extension.
    
    Args:
        A_G (np.ndarray): Host graph adjacency matrix (N x N)
                         A_G[i,j] = number of directed edges from i to j
        A_P (np.ndarray): Pattern graph adjacency matrix (k x k)
        n (int): Target number of isomorphic copies
        
    Returns:
        tuple: (A_curr, total_cost)
    """
    A_curr = A_G.copy()
    total_cost = 0
    used_vertex_sets = set()
    
    N = A_G.shape[0]
    k = A_P.shape[0]
    
    # Use total degree (in + out) as heuristic
    P_degrees = calculate_total_degrees(A_P)
    
    print(f"Starting Algorithm v.3 (DIRECTED): Finding {n} copies of pattern (k={k}) in graph (N={N})")
    print(f"Pattern total degrees (in+out): {P_degrees}")
    print()
    
    for i in range(n):
        print(f"--- Iteration {i+1}/{n} ---")
        
        G_degrees = calculate_total_degrees(A_curr)
        
        # For very small k, enumerate all possible mappings and pick best
        # This guarantees optimal solution
        if k <= 3 and N <= 10:
            print(f"  Using exhaustive search (k={k}, N={N})")
            from itertools import combinations, permutations
            
            # Generate all possible k-element subsets of N vertices
            best_mapping = None
            best_cost = float('inf')
            
            for vertex_set in combinations(range(N), k):
                vertex_set_frozen = frozenset(vertex_set)
                if vertex_set_frozen in used_vertex_sets:
                    continue
                
                # Try all permutations of this vertex set
                for perm in permutations(vertex_set):
                    mapping = np.array(perm)
                    cost = calculate_extension_cost_directed(A_curr, A_P, mapping)
                    if cost < best_cost:
                        best_cost = cost
                        best_mapping = mapping
            
            if best_mapping is None:
                raise RuntimeError("Unable to find sufficient unique mappings")
            
            mapping = best_mapping
            target_set = frozenset(mapping)
            used_vertex_sets.add(target_set)
            print(f"  Found optimal mapping: {mapping} (cost: {best_cost})")
            
        else:
            # Use assignment problem heuristic for larger k
            if k <= 4:
                cost_matrix = build_cost_matrix_quadratic_directed(A_curr, A_P, k, N, used_vertex_sets)
                print(f"  Using quadratic cost matrix (k={k})")
            else:
                cost_matrix = build_cost_matrix_directed(P_degrees, G_degrees, k, N)
                print(f"  Using linear cost matrix (k={k})")
            
            j = 1
            while True:
                mapping = k_best_assignment_solver(cost_matrix, j, k)
                target_set = frozenset(mapping)
                
                if target_set not in used_vertex_sets:
                    used_vertex_sets.add(target_set)
                    print(f"  Found unique mapping (attempt {j}): {mapping}")
                    break
                
                print(f"  Mapping attempt {j} already used: {set(mapping)}")
                j += 1
                
                if j > 1000:
                    print(f"  ERROR: Could not find unique mapping after {j} attempts!")
                    raise RuntimeError("Unable to find sufficient unique mappings")
        
        iteration_cost = calculate_extension_cost_directed(A_curr, A_P, mapping)
        print(f"  Extension cost: {iteration_cost}")
        
        A_curr = apply_extension_directed(A_curr, A_P, mapping)
        total_cost += iteration_cost
        print(f"  Total cost so far: {total_cost}")
        print()
    
    print(f"Algorithm complete. Total edges added: {total_cost}")
    return A_curr, total_cost


# ============================================================================
# EXAMPLES FOR DIRECTED MULTIGRAPHS
# ============================================================================

def example_directed_1():
    """Example 1: Simple directed edge"""
    print("=" * 70)
    print("DIRECTED Example 1: Single directed edge")
    print("=" * 70)
    
    # Host: 4 vertices, some directed edges
    A_G = np.array([
        [0, 1, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0]
    ])
    
    # Pattern: Single directed edge (u -> v)
    A_P = np.array([
        [0, 1],
        [0, 0]
    ])
    
    print("\nHost graph G (directed):")
    print("  0 → 1 (1 edge)")
    print("  1 → 2 (1 edge)")
    print("\nMatrix G:")
    print(A_G)
    
    print("\nPattern P (directed edge):")
    print("  0 → 1 (1 edge)")
    print("\nMatrix P:")
    print(A_P)
    
    n = 2
    print(f"\nFinding {n} copies...\n")
    
    A_result, cost = find_minimal_extension_v3_directed(A_G, A_P, n)
    
    print("\nFinal graph:")
    print(A_result)
    print(f"\nTotal cost: {cost} edges")
    return A_result, cost


def example_directed_2():
    """Example 2: Directed triangle (cycle)"""
    print("\n" + "=" * 70)
    print("DIRECTED Example 2: Directed triangle (3-cycle)")
    print("=" * 70)
    
    # Host: 5 vertices
    A_G = np.array([
        [0, 1, 0, 0, 0],
        [0, 0, 1, 0, 0],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1],
        [0, 0, 0, 0, 0]
    ])
    
    # Pattern: Directed triangle (0→1→2→0)
    A_P = np.array([
        [0, 1, 0],
        [0, 0, 1],
        [1, 0, 0]
    ])
    
    print("\nHost graph G:")
    print("  0 → 1 → 2")
    print("  3 → 4")
    print("\nMatrix G:")
    print(A_G)
    
    print("\nPattern P (directed cycle):")
    print("  0 → 1 → 2 → 0")
    print("\nMatrix P:")
    print(A_P)
    
    n = 1
    print(f"\nFinding {n} copy...\n")
    
    A_result, cost = find_minimal_extension_v3_directed(A_G, A_P, n)
    
    print("\nFinal graph:")
    print(A_result)
    print(f"\nTotal cost: {cost} edges")
    return A_result, cost


def example_directed_3():
    """Example 3: Multiple directed edges (multigraph)"""
    print("\n" + "=" * 70)
    print("DIRECTED Example 3: Multiple directed edges")
    print("=" * 70)
    
    # Host: 4 vertices with some double edges
    A_G = np.array([
        [0, 2, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0]
    ])
    
    # Pattern: 3 parallel directed edges from u to v
    A_P = np.array([
        [0, 3],
        [0, 0]
    ])
    
    print("\nHost graph G:")
    print("  0 ⇉ 1 (2 edges)")
    print("  1 → 2 (1 edge)")
    print("\nMatrix G:")
    print(A_G)
    
    print("\nPattern P:")
    print("  0 ⇶ 1 (3 parallel directed edges)")
    print("\nMatrix P:")
    print(A_P)
    
    n = 2
    print(f"\nFinding {n} copies...\n")
    
    A_result, cost = find_minimal_extension_v3_directed(A_G, A_P, n)
    
    print("\nFinal graph:")
    print(A_result)
    print(f"\nTotal cost: {cost} edges")
    return A_result, cost


def example_directed_4():
    """Example 4: Bidirectional edges with different multiplicities"""
    print("\n" + "=" * 70)
    print("DIRECTED Example 4: Asymmetric bidirectional edges")
    print("=" * 70)
    
    # Host: 4 vertices
    A_G = np.array([
        [0, 1, 0, 0],
        [2, 0, 0, 0],
        [0, 0, 0, 1],
        [0, 0, 3, 0]
    ])
    
    # Pattern: u→v with multiplicity 2, v→u with multiplicity 1
    A_P = np.array([
        [0, 2],
        [1, 0]
    ])
    
    print("\nHost graph G:")
    print("  0 → 1 (1 edge)")
    print("  1 → 0 (2 edges)")
    print("  2 → 3 (1 edge)")
    print("  3 → 2 (3 edges)")
    print("\nMatrix G:")
    print(A_G)
    
    print("\nPattern P:")
    print("  0 ⇉ 1 (2 edges)")
    print("  1 → 0 (1 edge)")
    print("\nMatrix P:")
    print(A_P)
    
    n = 2
    print(f"\nFinding {n} copies...\n")
    
    A_result, cost = find_minimal_extension_v3_directed(A_G, A_P, n)
    
    print("\nFinal graph:")
    print(A_result)
    print(f"\nTotal cost: {cost} edges")
    return A_result, cost


def example_directed_5():
    """Example 5: Self-loops in directed multigraph"""
    print("\n" + "=" * 70)
    print("DIRECTED Example 5: Self-loops")
    print("=" * 70)
    
    # Host: 4 vertices with one self-loop
    A_G = np.array([
        [2, 1, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 1]
    ])
    
    # Pattern: vertex with self-loop and outgoing edge
    A_P = np.array([
        [1, 1],
        [0, 0]
    ])
    
    print("\nHost graph G:")
    print("  0 ⟲ (2 self-loops)")
    print("  0 → 1 (1 edge)")
    print("  1 → 2 (1 edge)")
    print("  3 ⟲ (1 self-loop)")
    print("\nMatrix G:")
    print(A_G)
    
    print("\nPattern P:")
    print("  0 ⟲ (1 self-loop)")
    print("  0 → 1 (1 edge)")
    print("\nMatrix P:")
    print(A_P)
    
    n = 2
    print(f"\nFinding {n} copies...\n")
    
    A_result, cost = find_minimal_extension_v3_directed(A_G, A_P, n)
    
    print("\nFinal graph:")
    print(A_result)
    print(f"\nTotal cost: {cost} edges")
    return A_result, cost


def example_directed_6():
    """Example 6: Complex directed pattern"""
    print("\n" + "=" * 70)
    print("DIRECTED Example 6: Star pattern (one hub, directed outward)")
    print("=" * 70)
    
    # Host: 6 vertices
    A_G = np.array([
        [0, 1, 0, 0, 0, 0],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0]
    ])
    
    # Pattern: Hub with 3 outgoing edges
    A_P = np.array([
        [0, 1, 1, 1],
        [0, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0]
    ])
    
    print("\nHost graph G:")
    print("  0 → 1 → 2")
    print("  3 → 4")
    print("\nMatrix G:")
    print(A_G)
    
    print("\nPattern P (directed star):")
    print("  0 → 1, 0 → 2, 0 → 3")
    print("\nMatrix P:")
    print(A_P)
    
    n = 1
    print(f"\nFinding {n} copy...\n")
    
    A_result, cost = find_minimal_extension_v3_directed(A_G, A_P, n)
    
    print("\nFinal graph:")
    print(A_result)
    print(f"\nTotal cost: {cost} edges")
    return A_result, cost


def example_directed_7():
    """Example 7: Dense directed multigraph"""
    print("\n" + "=" * 70)
    print("DIRECTED Example 7: Dense pattern with various multiplicities")
    print("=" * 70)
    
    # Host: 5 vertices, sparse
    A_G = np.array([
        [0, 1, 0, 0, 0],
        [1, 0, 1, 0, 0],
        [0, 0, 0, 1, 0],
        [0, 0, 0, 0, 1],
        [0, 0, 0, 0, 0]
    ])
    
    # Pattern: Complex directed pattern
    A_P = np.array([
        [0, 2, 1],
        [1, 0, 3],
        [0, 1, 0]
    ])
    
    print("\nHost graph G:")
    print("\nMatrix G:")
    print(A_G)
    
    print("\nPattern P (complex directed):")
    print("  0 ⇉ 1 (2 edges), 0 → 2 (1 edge)")
    print("  1 → 0 (1 edge), 1 ⇶ 2 (3 edges)")
    print("  2 → 1 (1 edge)")
    print("\nMatrix P:")
    print(A_P)
    
    n = 1
    print(f"\nFinding {n} copy...\n")
    
    A_result, cost = find_minimal_extension_v3_directed(A_G, A_P, n)
    
    print("\nFinal graph:")
    print(A_result)
    print(f"\nTotal cost: {cost} edges")
    return A_result, cost


if __name__ == "__main__":
    print("\n")
    print("╔" + "═" * 68 + "╗")
    print("║" + " " * 10 + "ALGORITHM v.3 - DIRECTED MULTIGRAPH EXAMPLES" + " " * 14 + "║")
    print("╚" + "═" * 68 + "╝")
    
    examples = [
        example_directed_1,
        example_directed_2,
        example_directed_3,
        example_directed_4,
        example_directed_5,
        example_directed_6,
        example_directed_7
    ]
    
    for example_func in examples:
        try:
            example_func()
            print("\n" + "─" * 70)
        except Exception as e:
            print(f"\n⚠️  Error: {e}")
            import traceback
            traceback.print_exc()
            print("─" * 70)
    
    print("\n" + "╔" + "═" * 68 + "╗")
    print("║" + " " * 20 + "ALL EXAMPLES COMPLETE!" + " " * 27 + "║")
    print("╚" + "═" * 68 + "╝")
