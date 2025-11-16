#include "algorithms/subgraph_algorithm.h"
#include "graph/multigraph.h"
#include <vector>
#include <gtest/gtest.h>

using namespace Subgraphs;

template <typename T> class SubgraphAlgorithmTest : public ::testing::Test {
  protected:
    void SetUp() override {
    }
    void TearDown() override {
    }
};

using AlgorithmTypes = ::testing::Types<int32_t, int64_t>;
TYPED_TEST_SUITE(SubgraphAlgorithmTest, AlgorithmTypes);

TYPED_TEST(SubgraphAlgorithmTest, SimpleExtension) {
    std::vector<std::vector<uint8_t>> patternMatrix = {{0, 1}, {0, 0}};
    Multigraph<TypeParam> P(std::move(patternMatrix));

    std::vector<std::vector<uint8_t>> targetMatrix = {{0, 0}, {0, 0}};
    Multigraph<TypeParam> G(std::move(targetMatrix));

    auto result = SubgraphAlgorithm<TypeParam>::run(1, P, G);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];
    EXPECT_EQ(extension.size(), 1);
    EXPECT_EQ(extension[0].count, 1);
}

TYPED_TEST(SubgraphAlgorithmTest, NoExtensionNeeded) {
    // Pattern and target are identical
    std::vector<std::vector<uint8_t>> matrix1 = {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}};
    std::vector<std::vector<uint8_t>> matrix2 = {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}};
    Multigraph<TypeParam> P(std::move(matrix1));
    Multigraph<TypeParam> G(std::move(matrix2));

    auto result = SubgraphAlgorithm<TypeParam>::run(1, P, G);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];
    EXPECT_EQ(extension.size(), 0);
}

TYPED_TEST(SubgraphAlgorithmTest, MultipleEdgesToAdd) {
    std::vector<std::vector<uint8_t>> patternMatrix = {{0, 2, 1}, {1, 0, 0}, {0, 1, 0}};
    Multigraph<TypeParam> P(std::move(patternMatrix));

    std::vector<std::vector<uint8_t>> targetMatrix = {{0, 1, 0}, {0, 0, 0}, {0, 0, 0}};
    Multigraph<TypeParam> G(std::move(targetMatrix));

    auto result = SubgraphAlgorithm<TypeParam>::run(1, P, G);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];
    EXPECT_GT(extension.size(), 0);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_GT(totalEdges, 0);
}

TYPED_TEST(SubgraphAlgorithmTest, LargerTargetGraph) {
    std::vector<std::vector<uint8_t>> patternMatrix = {{0, 1}, {1, 0}};
    Multigraph<TypeParam> P(std::move(patternMatrix));

    std::vector<std::vector<uint8_t>> targetMatrix = {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    Multigraph<TypeParam> G(std::move(targetMatrix));

    auto result = SubgraphAlgorithm<TypeParam>::run(1, P, G);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());
}

TYPED_TEST(SubgraphAlgorithmTest, MultipleCopies) {
    std::vector<std::vector<uint8_t>> patternMatrix = {{0, 1}, {0, 0}};
    Multigraph<TypeParam> P(std::move(patternMatrix));

    std::vector<std::vector<uint8_t>> targetMatrix = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    Multigraph<TypeParam> G(std::move(targetMatrix));

    auto result = SubgraphAlgorithm<TypeParam>::run(2, P, G);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];
    EXPECT_GT(extension.size(), 0);
}

TYPED_TEST(SubgraphAlgorithmTest, SelfLoops) {
    std::vector<std::vector<uint8_t>> patternMatrix = {{1, 0}, {0, 0}};
    Multigraph<TypeParam> P(std::move(patternMatrix));

    std::vector<std::vector<uint8_t>> targetMatrix = {{0, 0}, {0, 0}};
    Multigraph<TypeParam> G(std::move(targetMatrix));

    auto result = SubgraphAlgorithm<TypeParam>::run(1, P, G);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];
    EXPECT_EQ(extension.size(), 1);

    bool foundSelfLoop = false;
    for (const auto& edge : extension) {
        if (edge.source == edge.destination) {
            foundSelfLoop = true;
            EXPECT_EQ(edge.count, 1);
        }
    }
    EXPECT_TRUE(foundSelfLoop);
}

TYPED_TEST(SubgraphAlgorithmTest, EdgeStructure) {
    std::vector<std::vector<uint8_t>> patternMatrix = {{0, 1}, {0, 0}};
    Multigraph<TypeParam> P(std::move(patternMatrix));

    std::vector<std::vector<uint8_t>> targetMatrix = {{0, 0}, {0, 0}};
    Multigraph<TypeParam> G(std::move(targetMatrix));

    auto result = SubgraphAlgorithm<TypeParam>::run(1, P, G);

    const auto& extension = result[0][0];

    for (const auto& edge : extension) {
        EXPECT_GE(edge.source, 0);
        EXPECT_LT(edge.source, G.getVertexCount());
        EXPECT_GE(edge.destination, 0);
        EXPECT_LT(edge.destination, G.getVertexCount());
        EXPECT_GT(edge.count, 0);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
