#include "graph/combination_iterator.h"
#include "graph/permutation_iterator.h"
#include "graph/sequence_iterator.h"
#include <algorithm>
#include <set>
#include <gtest/gtest.h>

using namespace Subgraphs;

// ============================================================================
// Permutation Iterator Tests
// ============================================================================

template <typename T> class PermutationIteratorTest : public ::testing::Test {};

using PermutationTypes = ::testing::Types<int32_t, int64_t>;
TYPED_TEST_SUITE(PermutationIteratorTest, PermutationTypes);

TYPED_TEST(PermutationIteratorTest, BasicIteration) {
    PermutationRange<TypeParam> range(3);

    std::set<std::vector<TypeParam>> uniquePerms;
    int count = 0;

    for (const auto& perm : range) {
        EXPECT_EQ(perm.size(), 3);
        uniquePerms.insert(perm);
        count++;
    }

    EXPECT_EQ(count, 6);
    EXPECT_EQ(uniquePerms.size(), 6);
}

TYPED_TEST(PermutationIteratorTest, AllPermutationsGenerated) {
    PermutationRange<TypeParam> range(3);
    std::set<std::vector<TypeParam>> uniquePerms;

    for (const auto& perm : range) {
        uniquePerms.insert(perm);
    }

    EXPECT_TRUE(uniquePerms.count({0, 1, 2}) == 1);
    EXPECT_TRUE(uniquePerms.count({0, 2, 1}) == 1);
    EXPECT_TRUE(uniquePerms.count({1, 0, 2}) == 1);
    EXPECT_TRUE(uniquePerms.count({1, 2, 0}) == 1);
    EXPECT_TRUE(uniquePerms.count({2, 0, 1}) == 1);
    EXPECT_TRUE(uniquePerms.count({2, 1, 0}) == 1);
}

TYPED_TEST(PermutationIteratorTest, SingleElement) {
    PermutationRange<TypeParam> range(1);
    int count = 0;

    for (const auto& perm : range) {
        EXPECT_EQ(perm.size(), 1);
        EXPECT_EQ(perm[0], 0);
        count++;
    }

    EXPECT_EQ(count, 1);
}

TYPED_TEST(PermutationIteratorTest, EmptySet) {
    PermutationRange<TypeParam> range(0);
    int count = 0;

    for (const auto& perm : range) {
        EXPECT_EQ(perm.size(), 0);
        count++;
    }

    EXPECT_EQ(count, 1);
}

TYPED_TEST(PermutationIteratorTest, LargerSet) {
    PermutationRange<TypeParam> range(4);
    int count = 0;

    for (const auto& perm : range) {
        EXPECT_EQ(perm.size(), 4);
        count++;
    }

    EXPECT_EQ(count, 24);
}

TYPED_TEST(PermutationIteratorTest, EarlyTermination) {
    PermutationRange<TypeParam> range(5);
    int count = 0;

    for (const auto& perm : range) {
        (void)perm;
        count++;
        if (count == 10)
            break;
    }

    EXPECT_EQ(count, 10);
}

// ============================================================================
// Combination Iterator Tests
// ============================================================================

template <typename T> class CombinationIteratorTest : public ::testing::Test {};

using CombinationTypes = ::testing::Types<int32_t, int64_t>;
TYPED_TEST_SUITE(CombinationIteratorTest, CombinationTypes);

TYPED_TEST(CombinationIteratorTest, BasicIteration) {
    CombinationRange<TypeParam> range(5, 3);

    std::set<std::vector<TypeParam>> uniqueCombs;
    int count = 0;

    for (const auto& comb : range) {
        EXPECT_EQ(comb.size(), 3);
        EXPECT_TRUE(std::is_sorted(comb.begin(), comb.end()));
        uniqueCombs.insert(comb);
        count++;
    }

    EXPECT_EQ(count, 10);
    EXPECT_EQ(uniqueCombs.size(), 10);
}

TYPED_TEST(CombinationIteratorTest, ElementsInRange) {
    CombinationRange<TypeParam> range(5, 3);

    for (const auto& comb : range) {
        for (auto elem : comb) {
            EXPECT_GE(elem, 0);
            EXPECT_LT(elem, 5);
        }
    }
}

TYPED_TEST(CombinationIteratorTest, UniqueElements) {
    CombinationRange<TypeParam> range(5, 3);

    for (const auto& comb : range) {
        std::set<TypeParam> uniqueElements(comb.begin(), comb.end());
        EXPECT_EQ(uniqueElements.size(), 3);
    }
}

TYPED_TEST(CombinationIteratorTest, KEqualsN) {
    CombinationRange<TypeParam> range(4, 4);
    int count = 0;

    for (const auto& comb : range) {
        EXPECT_EQ(comb.size(), 4);
        EXPECT_EQ(comb[0], 0);
        EXPECT_EQ(comb[1], 1);
        EXPECT_EQ(comb[2], 2);
        EXPECT_EQ(comb[3], 3);
        count++;
    }

    EXPECT_EQ(count, 1);
}

TYPED_TEST(CombinationIteratorTest, KEqualsOne) {
    CombinationRange<TypeParam> range(5, 1);
    int count = 0;

    for (const auto& comb : range) {
        EXPECT_EQ(comb.size(), 1);
        EXPECT_EQ(comb[0], count);
        count++;
    }

    EXPECT_EQ(count, 5);
}

TYPED_TEST(CombinationIteratorTest, InvalidKGreaterThanN) {
    CombinationRange<TypeParam> range(3, 5);
    int count = 0;

    for (const auto& comb : range) {
        (void)comb;
        count++;
    }

    EXPECT_EQ(count, 0);
}

TYPED_TEST(CombinationIteratorTest, KEqualsZero) {
    CombinationRange<TypeParam> range(5, 0);
    int count = 0;

    for (const auto& comb : range) {
        (void)comb;
        count++;
    }

    EXPECT_EQ(count, 0);
}

TYPED_TEST(CombinationIteratorTest, SpecificCombinations) {
    CombinationRange<TypeParam> range(5, 3);
    std::set<std::vector<TypeParam>> uniqueCombs;

    for (const auto& comb : range) {
        uniqueCombs.insert(comb);
    }

    EXPECT_TRUE(uniqueCombs.count({0, 1, 2}) == 1);
    EXPECT_TRUE(uniqueCombs.count({0, 1, 3}) == 1);
    EXPECT_TRUE(uniqueCombs.count({0, 1, 4}) == 1);
    EXPECT_TRUE(uniqueCombs.count({2, 3, 4}) == 1);
}

// ============================================================================
// Sequence Iterator Tests
// ============================================================================

template <typename T> class SequenceIteratorTest : public ::testing::Test {};

using SequenceTypes = ::testing::Types<int32_t, int64_t, uint32_t>;
TYPED_TEST_SUITE(SequenceIteratorTest, SequenceTypes);

TYPED_TEST(SequenceIteratorTest, BasicIteration) {
    SequenceRange<TypeParam> range(3, 2);

    int count = 0;
    for (const auto& seq : range) {
        EXPECT_EQ(seq.size(), 2);
        for (auto elem : seq) {
            EXPECT_GE(elem, 0);
            EXPECT_LT(elem, 3);
        }
        count++;
    }

    EXPECT_EQ(count, 9);
}

TYPED_TEST(SequenceIteratorTest, AllSequencesGenerated) {
    SequenceRange<TypeParam> range(2, 2);
    std::set<std::vector<TypeParam>> uniqueSeqs;

    for (const auto& seq : range) {
        uniqueSeqs.insert(seq);
    }

    EXPECT_EQ(uniqueSeqs.size(), 4);
    EXPECT_TRUE(uniqueSeqs.count({0, 0}) == 1);
    EXPECT_TRUE(uniqueSeqs.count({0, 1}) == 1);
    EXPECT_TRUE(uniqueSeqs.count({1, 0}) == 1);
    EXPECT_TRUE(uniqueSeqs.count({1, 1}) == 1);
}

TYPED_TEST(SequenceIteratorTest, LengthOne) {
    SequenceRange<TypeParam> range(5, 1);
    int count = 0;

    for (const auto& seq : range) {
        EXPECT_EQ(seq.size(), 1);
        EXPECT_EQ(seq[0], count);
        count++;
    }

    EXPECT_EQ(count, 5);
}

TYPED_TEST(SequenceIteratorTest, MaxValueOne) {
    SequenceRange<TypeParam> range(1, 3);
    int count = 0;

    for (const auto& seq : range) {
        EXPECT_EQ(seq.size(), 3);
        for (auto elem : seq) {
            EXPECT_EQ(elem, 0);
        }
        count++;
    }

    EXPECT_EQ(count, 1);
}

TYPED_TEST(SequenceIteratorTest, EmptyMaxValue) {
    SequenceRange<TypeParam> range(0, 3);
    int count = 0;

    for (const auto& seq : range) {
        (void)seq;
        count++;
    }

    EXPECT_EQ(count, 0);
}

TYPED_TEST(SequenceIteratorTest, EmptyLength) {
    SequenceRange<TypeParam> range(5, 0);
    int count = 0;

    for (const auto& seq : range) {
        (void)seq;
        count++;
    }

    EXPECT_EQ(count, 0);
}

TYPED_TEST(SequenceIteratorTest, LargerSequence) {
    SequenceRange<TypeParam> range(3, 3);
    int count = 0;

    for (const auto& seq : range) {
        EXPECT_EQ(seq.size(), 3);
        count++;
    }

    EXPECT_EQ(count, 27);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
