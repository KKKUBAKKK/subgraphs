#include "algorithms/subgraph_algorithm.h"
#include "graph/multigraph.h"
#include "utils/graph_loader.h"
#include <filesystem>
#include <gtest/gtest.h>

using namespace Subgraphs;

// Test fixture for sample graph tests
template <typename T> class SampleGraphTest : public ::testing::Test {
  protected:
    std::string getDataPath(const std::string& filename) {
        return std::filesystem::path(__FILE__).parent_path().parent_path() / "data" / filename;
    }
};

using SampleGraphTypes = ::testing::Types<int64_t>;
TYPED_TEST_SUITE(SampleGraphTest, SampleGraphTypes);

// ============================================================================
// Sample Graph 0 Tests
// ============================================================================

TYPED_TEST(SampleGraphTest, SampleGraph0_OneCopy) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs0.txt"));

    // Pattern: 3 vertices, Target: 4 vertices
    EXPECT_EQ(pattern.getVertexCount(), 3);
    EXPECT_EQ(target.getVertexCount(), 4);

    auto result = SubgraphAlgorithm<TypeParam>::run(1, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values after running the algorithm
    // Expected number of edges to add:
    EXPECT_EQ(extension.size(), 1);

    // Expected total edge count to add:
    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 2);
}

TYPED_TEST(SampleGraphTest, SampleGraph0_TwoCopies) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs0.txt"));

    auto result = SubgraphAlgorithm<TypeParam>::run(2, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values for 2 copies
    EXPECT_EQ(extension.size(), 2);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 3);
}

// ============================================================================
// Sample Graph 1 Tests
// ============================================================================

TYPED_TEST(SampleGraphTest, SampleGraph1_OneCopy) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs1.txt"));

    // Pattern: 4 vertices, Target: 6 vertices
    EXPECT_EQ(pattern.getVertexCount(), 4);
    EXPECT_EQ(target.getVertexCount(), 6);

    auto result = SubgraphAlgorithm<TypeParam>::run(1, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values
    EXPECT_EQ(extension.size(), 4);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 4);
}

TYPED_TEST(SampleGraphTest, SampleGraph1_TwoCopies) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs1.txt"));

    auto result = SubgraphAlgorithm<TypeParam>::run(2, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values for 2 copies
    EXPECT_EQ(extension.size(), 6);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 7);
}

// ============================================================================
// Sample Graph 2 Tests
// ============================================================================

TYPED_TEST(SampleGraphTest, SampleGraph2_OneCopy) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs2.txt"));

    // Pattern: 6 vertices, Target: 8 vertices
    EXPECT_EQ(pattern.getVertexCount(), 6);
    EXPECT_EQ(target.getVertexCount(), 8);

    auto result = SubgraphAlgorithm<TypeParam>::run(1, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values
    EXPECT_EQ(extension.size(), 3);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 3);
}

TYPED_TEST(SampleGraphTest, SampleGraph2_TwoCopies) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs2.txt"));

    auto result = SubgraphAlgorithm<TypeParam>::run(2, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values for 2 copies
    EXPECT_EQ(extension.size(), 0);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 0);
}

// ============================================================================
// Sample Graph 3 Tests
// ============================================================================

TYPED_TEST(SampleGraphTest, SampleGraph3_OneCopy) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs3.txt"));

    // Pattern: 7 vertices, Target: 12 vertices
    EXPECT_EQ(pattern.getVertexCount(), 7);
    EXPECT_EQ(target.getVertexCount(), 12);

    auto result = SubgraphAlgorithm<TypeParam>::run(1, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values
    EXPECT_EQ(extension.size(), 0);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 0);
}

TYPED_TEST(SampleGraphTest, SampleGraph3_TwoCopies) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs3.txt"));

    auto result = SubgraphAlgorithm<TypeParam>::run(2, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values for 2 copies
    EXPECT_EQ(extension.size(), 0);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 0);
}

// ============================================================================
// Sample Graph 4 Tests
// ============================================================================

TYPED_TEST(SampleGraphTest, SampleGraph4_OneCopy) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs4.txt"));

    // Pattern: 3 vertices, Target: 6 vertices
    EXPECT_EQ(pattern.getVertexCount(), 3);
    EXPECT_EQ(target.getVertexCount(), 6);

    auto result = SubgraphAlgorithm<TypeParam>::run(1, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values
    EXPECT_EQ(extension.size(), 0);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 0);
}

TYPED_TEST(SampleGraphTest, SampleGraph4_TwoCopies) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs4.txt"));

    auto result = SubgraphAlgorithm<TypeParam>::run(2, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values for 2 copies
    EXPECT_EQ(extension.size(), 0);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 0);
}

// ============================================================================
// Sample Graph 5 Tests
// ============================================================================

TYPED_TEST(SampleGraphTest, SampleGraph5_OneCopy) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs5.txt"));

    // Pattern: 3 vertices, Target: 8 vertices
    EXPECT_EQ(pattern.getVertexCount(), 3);
    EXPECT_EQ(target.getVertexCount(), 8);

    auto result = SubgraphAlgorithm<TypeParam>::run(1, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values
    EXPECT_EQ(extension.size(), 0);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 0);
}

TYPED_TEST(SampleGraphTest, SampleGraph5_TwoCopies) {
    auto [pattern, target] =
        GraphLoader<TypeParam>::loadFromFile(this->getDataPath("sample_graphs5.txt"));

    auto result = SubgraphAlgorithm<TypeParam>::run(2, pattern, target);

    ASSERT_FALSE(result.empty());
    ASSERT_FALSE(result[0].empty());

    const auto& extension = result[0][0];

    // TODO: Fill in expected values for 2 copies
    EXPECT_EQ(extension.size(), 0);

    int totalEdges = 0;
    for (const auto& edge : extension) {
        totalEdges += edge.count;
    }
    EXPECT_EQ(totalEdges, 0);
}

// ============================================================================
// Additional test: Verify all extensions produce valid results
// ============================================================================

TYPED_TEST(SampleGraphTest, AllSampleGraphs_ValidExtensions) {
    std::vector<std::string> graphFiles = {"sample_graphs0.txt", "sample_graphs1.txt",
                                           "sample_graphs2.txt", "sample_graphs3.txt",
                                           "sample_graphs4.txt", "sample_graphs5.txt"};

    for (const auto& filename : graphFiles) {
        auto [pattern, target] = GraphLoader<TypeParam>::loadFromFile(this->getDataPath(filename));

        auto result = SubgraphAlgorithm<TypeParam>::run(1, pattern, target);

        ASSERT_FALSE(result.empty()) << "No result for " << filename;
        ASSERT_FALSE(result[0].empty()) << "Empty result for " << filename;

        const auto& extension = result[0][0];

        // Verify all edges are within bounds
        for (const auto& edge : extension) {
            EXPECT_GE(edge.source, 0) << "Invalid source in " << filename;
            EXPECT_LT(edge.source, target.getVertexCount())
                << "Source out of bounds in " << filename;
            EXPECT_GE(edge.destination, 0) << "Invalid destination in " << filename;
            EXPECT_LT(edge.destination, target.getVertexCount())
                << "Destination out of bounds in " << filename;
            EXPECT_GT(edge.count, 0) << "Invalid edge count in " << filename;
        }
    }
}

// Main function
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
