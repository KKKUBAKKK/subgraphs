#include "graph/combination_iterator.h"
#include "graph/permutation_iterator.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>

void testPermutationIterator() {
    std::cout << "Testing: Permutation iterator...\n";

    // Test with n=3, should generate 3! = 6 permutations
    Subgraphs::PermutationRange<int64_t> range(3);

    std::set<std::vector<int64_t>> uniquePerms;
    int count = 0;

    for (const auto& perm : range) {
        assert(perm.size() == 3);
        uniquePerms.insert(perm);
        count++;
    }

    assert(count == 6); // 3! = 6
    assert(uniquePerms.size() == 6);

    // Verify all expected permutations are present
    assert(uniquePerms.count({0, 1, 2}) == 1);
    assert(uniquePerms.count({0, 2, 1}) == 1);
    assert(uniquePerms.count({1, 0, 2}) == 1);
    assert(uniquePerms.count({1, 2, 0}) == 1);
    assert(uniquePerms.count({2, 0, 1}) == 1);
    assert(uniquePerms.count({2, 1, 0}) == 1);

    std::cout << "✓ Passed\n\n";
}

void testPermutationSingleElement() {
    std::cout << "Testing: Permutation with single element...\n";

    Subgraphs::PermutationRange<int64_t> range(1);
    int count = 0;

    for (const auto& perm : range) {
        assert(perm.size() == 1);
        assert(perm[0] == 0);
        count++;
    }

    assert(count == 1);
    std::cout << "✓ Passed\n\n";
}

void testPermutationEmpty() {
    std::cout << "Testing: Permutation with empty set...\n";

    Subgraphs::PermutationRange<int64_t> range(0);
    int count = 0;

    for (const auto& perm : range) {
        (void)perm;
        count++;
    }

    assert(count == 1); // Empty permutation
    std::cout << "✓ Passed\n\n";
}

void testCombinationIterator() {
    std::cout << "Testing: Combination iterator...\n";

    // Test C(5, 3) = 10 combinations
    Subgraphs::CombinationRange<int64_t> range(5, 3);

    std::set<std::vector<int64_t>> uniqueCombs;
    int count = 0;

    for (const auto& comb : range) {
        assert(comb.size() == 3);
        // Verify combination is in sorted order
        assert(std::is_sorted(comb.begin(), comb.end()));
        // Verify all elements are unique
        std::set<int64_t> uniqueElements(comb.begin(), comb.end());
        assert(uniqueElements.size() == 3);
        // Verify all elements are in valid range
        for (auto elem : comb) {
            assert(elem >= 0 && elem < 5);
        }
        uniqueCombs.insert(comb);
        count++;
    }

    assert(count == 10); // C(5,3) = 10
    assert(uniqueCombs.size() == 10);

    // Verify some expected combinations
    assert(uniqueCombs.count({0, 1, 2}) == 1);
    assert(uniqueCombs.count({0, 1, 3}) == 1);
    assert(uniqueCombs.count({0, 1, 4}) == 1);
    assert(uniqueCombs.count({2, 3, 4}) == 1);

    std::cout << "✓ Passed\n\n";
}

void testCombinationKEqualsN() {
    std::cout << "Testing: Combination with k=n...\n";

    // C(4, 4) = 1 combination
    Subgraphs::CombinationRange<int64_t> range(4, 4);
    int count = 0;

    for (const auto& comb : range) {
        assert(comb.size() == 4);
        assert(comb[0] == 0);
        assert(comb[1] == 1);
        assert(comb[2] == 2);
        assert(comb[3] == 3);
        count++;
    }

    assert(count == 1);
    std::cout << "✓ Passed\n\n";
}

void testCombinationKEquals1() {
    std::cout << "Testing: Combination with k=1...\n";

    // C(5, 1) = 5 combinations
    Subgraphs::CombinationRange<int64_t> range(5, 1);
    int count = 0;

    for (const auto& comb : range) {
        assert(comb.size() == 1);
        assert(comb[0] == count);
        count++;
    }

    assert(count == 5);
    std::cout << "✓ Passed\n\n";
}

void testCombinationInvalid() {
    std::cout << "Testing: Combination with invalid k...\n";

    // k > n, should produce no combinations
    Subgraphs::CombinationRange<int64_t> range1(3, 5);
    int count1 = 0;
    for (const auto& comb : range1) {
        (void)comb;
        count1++;
    }
    assert(count1 == 0);

    // k = 0, should produce no combinations
    Subgraphs::CombinationRange<int64_t> range2(5, 0);
    int count2 = 0;
    for (const auto& comb : range2) {
        (void)comb;
        count2++;
    }
    assert(count2 == 0);

    std::cout << "✓ Passed\n\n";
}

void testLazyEvaluation() {
    std::cout << "Testing: Lazy evaluation (memory efficiency)...\n";

    // Create iterator for large n without generating all at once
    Subgraphs::PermutationRange<int64_t> permRange(10);
    auto permIt = permRange.begin();

    // Get first permutation without generating all 10! = 3,628,800 permutations
    auto firstPerm = *permIt; // Copy the first permutation
    assert(firstPerm.size() == 10);
    assert(firstPerm[0] == 0);
    assert(firstPerm[1] == 1);

    // Advance one step
    ++permIt;
    if (permIt != permRange.end()) {
        const auto& secondPerm = *permIt;
        assert(secondPerm != firstPerm);
    }

    // For combinations
    Subgraphs::CombinationRange<int64_t> combRange(10, 5);
    auto combIt = combRange.begin();

    // Get first combination without generating all C(10,5) = 252 combinations
    const auto& firstComb = *combIt;
    assert(firstComb.size() == 5);
    assert(firstComb[0] == 0);
    assert(firstComb[4] == 4);

    std::cout << "✓ Passed\n\n";
}

void testEarlyTermination() {
    std::cout << "Testing: Early termination...\n";

    // Test that we can stop iteration early without generating all
    Subgraphs::PermutationRange<int64_t> range(5);
    int count = 0;

    for (const auto& perm : range) {
        (void)perm;
        count++;
        if (count == 10) {
            break; // Stop after 10 permutations instead of all 120
        }
    }

    assert(count == 10);
    std::cout << "✓ Passed\n\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "Running Permutation & Combination Tests\n";
    std::cout << "========================================\n\n";

    try {
        testPermutationIterator();
        testPermutationSingleElement();
        testPermutationEmpty();
        testCombinationIterator();
        testCombinationKEqualsN();
        testCombinationKEquals1();
        testCombinationInvalid();
        testLazyEvaluation();
        testEarlyTermination();

        std::cout << "========================================\n";
        std::cout << "All tests passed! ✓\n";
        std::cout << "========================================\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << "\n";
        return 1;
    }
}
