"""
Test suite for Algorithm v.3: Minimal Multigraph Extension

This module provides comprehensive tests for the implementation, including:
- Unit tests for individual helper functions
- Integration tests for the main algorithm
- Edge case testing
- Performance benchmarks
"""

import numpy as np
import sys
import time
from algorithm_v3 import (
    calculate_degrees,
    build_cost_matrix,
    k_best_assignment_solver,
    calculate_extension_cost,
    apply_extension,
    find_minimal_extension_v3
)


class TestAlgorithmV3:
    """Test suite for Algorithm v.3"""
    
    def __init__(self):
        self.tests_passed = 0
        self.tests_failed = 0
        
    def assert_equal(self, actual, expected, test_name):
        """Assert that two values are equal"""
        if isinstance(actual, np.ndarray):
            if np.array_equal(actual, expected):
                print(f"✓ {test_name}")
                self.tests_passed += 1
                return True
            else:
                print(f"✗ {test_name}")
                print(f"  Expected: {expected}")
                print(f"  Got: {actual}")
                self.tests_failed += 1
                return False
        else:
            if actual == expected:
                print(f"✓ {test_name}")
                self.tests_passed += 1
                return True
            else:
                print(f"✗ {test_name}")
                print(f"  Expected: {expected}")
                print(f"  Got: {actual}")
                self.tests_failed += 1
                return False
    
    def assert_true(self, condition, test_name):
        """Assert that a condition is true"""
        if condition:
            print(f"✓ {test_name}")
            self.tests_passed += 1
            return True
        else:
            print(f"✗ {test_name}")
            self.tests_failed += 1
            return False
    
    def test_calculate_degrees(self):
        """Test degree calculation"""
        print("\n--- Testing calculate_degrees ---")
        
        # Test 1: Simple graph
        A = np.array([
            [0, 1, 1],
            [1, 0, 1],
            [1, 1, 0]
        ])
        degrees = calculate_degrees(A)
        self.assert_equal(degrees, np.array([2, 2, 2]), "Triangle degrees")
        
        # Test 2: Multigraph
        A = np.array([
            [0, 2, 1],
            [2, 0, 0],
            [1, 0, 0]
        ])
        degrees = calculate_degrees(A)
        self.assert_equal(degrees, np.array([3, 2, 1]), "Multigraph degrees")
        
        # Test 3: Isolated vertices
        A = np.array([
            [0, 0, 0],
            [0, 0, 1],
            [0, 1, 0]
        ])
        degrees = calculate_degrees(A)
        self.assert_equal(degrees, np.array([0, 1, 1]), "Graph with isolated vertex")
    
    def test_build_cost_matrix(self):
        """Test cost matrix construction"""
        print("\n--- Testing build_cost_matrix ---")
        
        P_degrees = np.array([2, 2])
        G_degrees = np.array([1, 2, 3, 0])
        k = 2
        N = 4
        
        cost_matrix = build_cost_matrix(P_degrees, G_degrees, k, N)
        
        # Check dimensions
        self.assert_true(cost_matrix.shape == (N, N), "Cost matrix dimensions")
        
        # Check specific costs (degree differences)
        expected_cost_00 = abs(2 - 1)  # P[0] to G[0]
        expected_cost_01 = abs(2 - 2)  # P[0] to G[1]
        self.assert_equal(cost_matrix[0, 0], expected_cost_00, "Cost[0,0] = |2-1| = 1")
        self.assert_equal(cost_matrix[0, 1], expected_cost_01, "Cost[0,1] = |2-2| = 0")
        
        # Check dummy rows
        self.assert_true(np.all(cost_matrix[k:, :] == 0), "Dummy rows are zero")
    
    def test_calculate_extension_cost(self):
        """Test extension cost calculation"""
        print("\n--- Testing calculate_extension_cost ---")
        
        # Current graph
        A_curr = np.array([
            [0, 1, 0],
            [1, 0, 0],
            [0, 0, 0]
        ])
        
        # Pattern (triangle)
        A_P = np.array([
            [0, 1, 1],
            [1, 0, 1],
            [1, 1, 0]
        ])
        
        # Mapping: 0->0, 1->1, 2->2
        mapping = np.array([0, 1, 2])
        
        cost = calculate_extension_cost(A_curr, A_P, mapping)
        # Need to add: (0,2), (1,2) = 2 edges
        self.assert_equal(cost, 2, "Cost to complete triangle")
        
        # Test with existing edges
        A_curr2 = np.array([
            [0, 1, 1],
            [1, 0, 1],
            [1, 1, 0]
        ])
        cost2 = calculate_extension_cost(A_curr2, A_P, mapping)
        self.assert_equal(cost2, 0, "Cost for existing triangle")
    
    def test_apply_extension(self):
        """Test extension application"""
        print("\n--- Testing apply_extension ---")
        
        # Current graph
        A_curr = np.array([
            [0, 1, 0],
            [1, 0, 0],
            [0, 0, 0]
        ])
        
        # Pattern (triangle)
        A_P = np.array([
            [0, 1, 1],
            [1, 0, 1],
            [1, 1, 0]
        ])
        
        mapping = np.array([0, 1, 2])
        
        A_new = apply_extension(A_curr, A_P, mapping)
        
        # Check that edges were added
        expected = np.array([
            [0, 1, 1],
            [1, 0, 1],
            [1, 1, 0]
        ])
        self.assert_equal(A_new, expected, "Triangle extension")
        
        # Check symmetry
        self.assert_true(np.array_equal(A_new, A_new.T), "Extended graph is symmetric")
        
        # Check original not modified
        original = np.array([
            [0, 1, 0],
            [1, 0, 0],
            [0, 0, 0]
        ])
        self.assert_equal(A_curr, original, "Original graph unchanged")
    
    def test_multigraph_extension(self):
        """Test extension with multiple edges"""
        print("\n--- Testing multigraph extension ---")
        
        A_curr = np.array([
            [0, 1],
            [1, 0]
        ])
        
        A_P = np.array([
            [0, 3],
            [3, 0]
        ])
        
        mapping = np.array([0, 1])
        
        cost = calculate_extension_cost(A_curr, A_P, mapping)
        self.assert_equal(cost, 2, "Cost to add 2 more edges")
        
        A_new = apply_extension(A_curr, A_P, mapping)
        expected = np.array([
            [0, 3],
            [3, 0]
        ])
        self.assert_equal(A_new, expected, "Multigraph extension")
    
    def test_main_algorithm_triangle(self):
        """Test main algorithm with triangle pattern"""
        print("\n--- Testing main algorithm: Triangle ---")
        
        # Host graph
        A_G = np.array([
            [0, 1, 0, 0],
            [1, 0, 1, 0],
            [0, 1, 0, 1],
            [0, 0, 1, 0]
        ])
        
        # Pattern (triangle)
        A_P = np.array([
            [0, 1, 1],
            [1, 0, 1],
            [1, 1, 0]
        ])
        
        n = 1
        A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
        
        # Check that cost is non-negative
        self.assert_true(cost >= 0, "Cost is non-negative")
        
        # Check that result is symmetric
        self.assert_true(np.array_equal(A_result, A_result.T), 
                        "Result is symmetric")
        
        # Check that no edges were removed
        self.assert_true(np.all(A_result >= A_G), 
                        "No edges removed from original graph")
    
    def test_main_algorithm_multiple_copies(self):
        """Test main algorithm with multiple copies"""
        print("\n--- Testing main algorithm: Multiple copies ---")
        
        # Host graph (6 vertices)
        A_G = np.array([
            [0, 1, 0, 0, 0, 0],
            [1, 0, 0, 0, 0, 0],
            [0, 0, 0, 1, 0, 0],
            [0, 0, 1, 0, 0, 0],
            [0, 0, 0, 0, 0, 1],
            [0, 0, 0, 0, 1, 0]
        ])
        
        # Pattern (edge)
        A_P = np.array([
            [0, 1],
            [1, 0]
        ])
        
        n = 3
        A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
        
        # Check basic properties
        self.assert_true(cost >= 0, "Cost is non-negative")
        self.assert_true(np.array_equal(A_result, A_result.T), 
                        "Result is symmetric")
        self.assert_true(np.all(A_result >= A_G), 
                        "No edges removed")
        
        print(f"  Found {n} copies with cost {cost}")
    
    def test_edge_cases(self):
        """Test edge cases"""
        print("\n--- Testing edge cases ---")
        
        # Test 1: Pattern equals host graph
        A_G = np.array([
            [0, 1],
            [1, 0]
        ])
        A_P = np.array([
            [0, 1],
            [1, 0]
        ])
        n = 1
        
        A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
        self.assert_equal(cost, 0, "No cost when pattern already exists")
        self.assert_equal(A_result, A_G, "Graph unchanged")
        
        # Test 2: Empty pattern (single vertex)
        A_G = np.array([
            [0, 0],
            [0, 0]
        ])
        A_P = np.array([[0]])
        n = 2
        
        A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
        self.assert_equal(cost, 0, "No cost for isolated vertices")
    
    def test_self_loops(self):
        """Test graphs with self-loops"""
        print("\n--- Testing self-loops ---")
        
        A_G = np.array([
            [0, 1, 0],
            [1, 0, 1],
            [0, 1, 0]
        ])
        
        # Pattern with self-loop
        A_P = np.array([
            [1, 1],
            [1, 0]
        ])
        
        n = 1
        A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
        
        self.assert_true(cost >= 0, "Cost is non-negative")
        print(f"  Self-loop pattern cost: {cost}")
    
    def run_all_tests(self):
        """Run all test suites"""
        print("=" * 60)
        print("Running Algorithm v.3 Test Suite")
        print("=" * 60)
        
        self.test_calculate_degrees()
        self.test_build_cost_matrix()
        self.test_calculate_extension_cost()
        self.test_apply_extension()
        self.test_multigraph_extension()
        self.test_main_algorithm_triangle()
        self.test_main_algorithm_multiple_copies()
        self.test_edge_cases()
        self.test_self_loops()
        
        print("\n" + "=" * 60)
        print(f"Tests Passed: {self.tests_passed}")
        print(f"Tests Failed: {self.tests_failed}")
        print("=" * 60)
        
        return self.tests_failed == 0


def performance_benchmark():
    """Run performance benchmarks"""
    print("\n" + "=" * 60)
    print("Performance Benchmarks")
    print("=" * 60)
    
    # Benchmark 1: Varying graph size
    print("\nBenchmark 1: Varying graph size (k=3, n=2)")
    for N in [10, 20, 30, 50]:
        # Create random sparse graph
        A_G = np.random.randint(0, 2, size=(N, N))
        A_G = (A_G + A_G.T) // 2  # Make symmetric
        np.fill_diagonal(A_G, 0)  # Remove self-loops
        
        # Pattern (triangle)
        A_P = np.array([
            [0, 1, 1],
            [1, 0, 1],
            [1, 1, 0]
        ])
        
        n = 2
        
        start_time = time.time()
        A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
        elapsed = time.time() - start_time
        
        print(f"  N={N:2d}: {elapsed:.4f}s, cost={cost}")
    
    # Benchmark 2: Varying number of copies
    print("\nBenchmark 2: Varying n (N=20, k=3)")
    N = 20
    A_G = np.random.randint(0, 2, size=(N, N))
    A_G = (A_G + A_G.T) // 2
    np.fill_diagonal(A_G, 0)
    
    A_P = np.array([
        [0, 1, 1],
        [1, 0, 1],
        [1, 1, 0]
    ])
    
    for n in [1, 2, 3, 5]:
        start_time = time.time()
        try:
            A_result, cost = find_minimal_extension_v3(A_G, A_P, n)
            elapsed = time.time() - start_time
            print(f"  n={n}: {elapsed:.4f}s, cost={cost}")
        except RuntimeError as e:
            print(f"  n={n}: Failed - {e}")


if __name__ == "__main__":
    # Run tests
    test_suite = TestAlgorithmV3()
    success = test_suite.run_all_tests()
    
    # Run benchmarks if tests pass
    if success:
        performance_benchmark()
        sys.exit(0)
    else:
        print("\n⚠️  Some tests failed. Skipping benchmarks.")
        sys.exit(1)
