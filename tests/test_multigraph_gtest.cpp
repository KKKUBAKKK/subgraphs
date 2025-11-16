#include "graph/multigraph.h"
#include <vector>
#include <gtest/gtest.h>

using namespace Subgraphs;

// Test fixture for Multigraph tests
template <typename T> class MultigraphTest : public ::testing::Test {
  protected:
    void SetUp() override {
    }
    void TearDown() override {
    }
};

// Test with different types
using TestTypes = ::testing::Types<int32_t, int64_t, uint32_t>;
TYPED_TEST_SUITE(MultigraphTest, TestTypes);

TYPED_TEST(MultigraphTest, ConstructorWithSize) {
    Multigraph<TypeParam> graph(5);
    EXPECT_EQ(graph.getVertexCount(), 5);
    EXPECT_EQ(graph.getEdgeCount(), 0);
}

TYPED_TEST(MultigraphTest, ConstructorWithMatrix) {
    std::vector<std::vector<uint8_t>> matrix = {{0, 1, 0}, {2, 0, 1}, {0, 0, 0}};
    Multigraph<TypeParam> graph(std::move(matrix));
    EXPECT_EQ(graph.getVertexCount(), 3);
    EXPECT_EQ(graph.getEdgeCount(), 4);
}

TYPED_TEST(MultigraphTest, AddEdges) {
    Multigraph<TypeParam> graph(3);
    graph.addEdges(0, 1, 2);
    graph.addEdges(1, 2, 1);

    EXPECT_EQ(graph.getEdges(0, 1), 2);
    EXPECT_EQ(graph.getEdges(1, 2), 1);
    EXPECT_EQ(graph.getEdges(2, 0), 0);
    EXPECT_EQ(graph.getEdgeCount(), 3);
}

TYPED_TEST(MultigraphTest, GetEdges) {
    std::vector<std::vector<uint8_t>> matrix = {{0, 3, 0}, {1, 0, 2}, {0, 0, 0}};
    Multigraph<TypeParam> graph(std::move(matrix));

    EXPECT_EQ(graph.getEdges(0, 1), 3);
    EXPECT_EQ(graph.getEdges(1, 0), 1);
    EXPECT_EQ(graph.getEdges(1, 2), 2);
    EXPECT_EQ(graph.getEdges(2, 1), 0);
}

TYPED_TEST(MultigraphTest, GetInDegree) {
    std::vector<std::vector<uint8_t>> matrix = {{0, 1, 2}, {1, 0, 1}, {0, 0, 0}};
    Multigraph<TypeParam> graph(std::move(matrix));

    EXPECT_EQ(graph.getInDegree(0), 1);
    EXPECT_EQ(graph.getInDegree(1), 1);
    EXPECT_EQ(graph.getInDegree(2), 3);
}

TYPED_TEST(MultigraphTest, GetOutDegree) {
    std::vector<std::vector<uint8_t>> matrix = {{0, 1, 2}, {1, 0, 1}, {0, 0, 0}};
    Multigraph<TypeParam> graph(std::move(matrix));

    EXPECT_EQ(graph.getOutDegree(0), 3);
    EXPECT_EQ(graph.getOutDegree(1), 2);
    EXPECT_EQ(graph.getOutDegree(2), 0);
}

TYPED_TEST(MultigraphTest, GetDegree) {
    std::vector<std::vector<uint8_t>> matrix = {{0, 1, 2}, {1, 0, 1}, {0, 0, 0}};
    Multigraph<TypeParam> graph(std::move(matrix));

    auto degree0 = graph.getDegree(0);
    EXPECT_EQ(degree0.inDegree, 1);
    EXPECT_EQ(degree0.outDegree, 3);

    auto degree2 = graph.getDegree(2);
    EXPECT_EQ(degree2.inDegree, 3);
    EXPECT_EQ(degree2.outDegree, 0);
}

TYPED_TEST(MultigraphTest, PermutationsCount) {
    Multigraph<TypeParam> graph3(3);
    EXPECT_EQ(graph3.permutationsCount(), 6); // 3! = 6

    Multigraph<TypeParam> graph4(4);
    EXPECT_EQ(graph4.permutationsCount(), 24); // 4! = 24

    Multigraph<TypeParam> graph1(1);
    EXPECT_EQ(graph1.permutationsCount(), 1);
}

TYPED_TEST(MultigraphTest, CombinationsCount) {
    Multigraph<TypeParam> graph(5);

    EXPECT_EQ(graph.combinationsCount(2), 10); // C(5,2) = 10
    EXPECT_EQ(graph.combinationsCount(3), 10); // C(5,3) = 10
    EXPECT_EQ(graph.combinationsCount(5), 1);  // C(5,5) = 1
    EXPECT_EQ(graph.combinationsCount(0), 1);  // C(5,0) = 1
    EXPECT_EQ(graph.combinationsCount(6), 0);  // C(5,6) = 0
}

TYPED_TEST(MultigraphTest, CopyConstructor) {
    std::vector<std::vector<uint8_t>> matrix = {{0, 1, 2}, {1, 0, 1}, {0, 0, 0}};
    Multigraph<TypeParam> graph1(std::move(matrix));
    Multigraph<TypeParam> graph2(graph1);

    EXPECT_EQ(graph2.getVertexCount(), graph1.getVertexCount());
    EXPECT_EQ(graph2.getEdgeCount(), graph1.getEdgeCount());
    EXPECT_EQ(graph2.getEdges(0, 1), graph1.getEdges(0, 1));
}

TYPED_TEST(MultigraphTest, ComparisonOperators) {
    Multigraph<TypeParam> graph1(3);
    Multigraph<TypeParam> graph2(3);
    Multigraph<TypeParam> graph3(4);

    EXPECT_TRUE(graph1 == graph2);
    EXPECT_FALSE(graph1 != graph2);
    EXPECT_FALSE(graph1 == graph3);
    EXPECT_TRUE(graph1 != graph3);
    EXPECT_TRUE(graph1 < graph3);
}

TYPED_TEST(MultigraphTest, MultipleEdges) {
    Multigraph<TypeParam> graph(2);

    graph.addEdges(0, 1, 5);
    EXPECT_EQ(graph.getEdges(0, 1), 5);

    graph.addEdges(0, 1, 3);
    EXPECT_EQ(graph.getEdges(0, 1), 8);
    EXPECT_EQ(graph.getEdgeCount(), 8);
}

TYPED_TEST(MultigraphTest, GetInNeighbors) {
    std::vector<std::vector<uint8_t>> matrix = {{0, 1, 2}, {3, 0, 0}, {0, 1, 0}};
    Multigraph<TypeParam> graph(std::move(matrix));

    auto neighbors = graph.getInNeighbors(1);
    EXPECT_EQ(neighbors.size(), 2);

    bool found0 = false, found2 = false;
    for (const auto& [vertex, weight] : neighbors) {
        if (vertex == 0 && weight == 1)
            found0 = true;
        if (vertex == 2 && weight == 1)
            found2 = true;
    }
    EXPECT_TRUE(found0);
    EXPECT_TRUE(found2);
}

TYPED_TEST(MultigraphTest, GetOutNeighbors) {
    std::vector<std::vector<uint8_t>> matrix = {{0, 1, 2}, {3, 0, 0}, {0, 1, 0}};
    Multigraph<TypeParam> graph(std::move(matrix));

    auto neighbors = graph.getOutNeighbors(0);
    EXPECT_EQ(neighbors.size(), 2);

    bool found1 = false, found2 = false;
    for (const auto& [vertex, weight] : neighbors) {
        if (vertex == 1 && weight == 1)
            found1 = true;
        if (vertex == 2 && weight == 2)
            found2 = true;
    }
    EXPECT_TRUE(found1);
    EXPECT_TRUE(found2);
}

// Main function
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
