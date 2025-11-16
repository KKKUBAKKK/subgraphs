#include "graph/multigraph.h"
#include "utils/graph_loader.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

using namespace Subgraphs;

template <typename T> class GraphLoaderTest : public ::testing::Test {
  protected:
    std::string testFilePath;

    void SetUp() override {
        testFilePath = "test_graph_temp.txt";
    }

    void TearDown() override {
        if (std::filesystem::exists(testFilePath)) {
            std::filesystem::remove(testFilePath);
        }
    }

    void createTestFile(const std::string& content) {
        std::ofstream file(testFilePath);
        file << content;
        file.close();
    }
};

using LoaderTypes = ::testing::Types<int32_t, int64_t>;
TYPED_TEST_SUITE(GraphLoaderTest, LoaderTypes);

TYPED_TEST(GraphLoaderTest, LoadSimpleGraph) {
    this->createTestFile(R"(3
0 1 0
2 0 1
1 0 0
4
0 2 0 0
0 0 1 0
1 0 0 1
0 0 0 0
)");

    auto [pattern, target] = GraphLoader<TypeParam>::loadFromFile(this->testFilePath);

    EXPECT_EQ(pattern.getVertexCount(), 3);
    EXPECT_EQ(target.getVertexCount(), 4);
    EXPECT_LT(pattern.getVertexCount(), target.getVertexCount());
}

TYPED_TEST(GraphLoaderTest, LoadGraphWithEdges) {
    this->createTestFile(R"(2
0 3
1 0
3
0 1 0
0 0 2
1 0 0
)");

    auto [pattern, target] = GraphLoader<TypeParam>::loadFromFile(this->testFilePath);

    // Smaller graph should be pattern
    EXPECT_EQ(pattern.getVertexCount(), 2);
    EXPECT_EQ(target.getVertexCount(), 3);

    EXPECT_EQ(pattern.getEdges(0, 1), 3);
    EXPECT_EQ(pattern.getEdges(1, 0), 1);
}

TYPED_TEST(GraphLoaderTest, LoadIdenticalSizeGraphs) {
    this->createTestFile(R"(2
0 1
0 0
2
0 0
1 0
)");

    auto [pattern, target] = GraphLoader<TypeParam>::loadFromFile(this->testFilePath);

    EXPECT_EQ(pattern.getVertexCount(), 2);
    EXPECT_EQ(target.getVertexCount(), 2);
}

TYPED_TEST(GraphLoaderTest, LoadSingleVertex) {
    this->createTestFile(R"(1
0
2
0 1
0 0
)");

    auto [pattern, target] = GraphLoader<TypeParam>::loadFromFile(this->testFilePath);

    EXPECT_EQ(pattern.getVertexCount(), 1);
    EXPECT_EQ(target.getVertexCount(), 2);
}

TYPED_TEST(GraphLoaderTest, InvalidFileThrows) {
    EXPECT_THROW(GraphLoader<TypeParam>::loadFromFile("nonexistent_file.txt"), std::runtime_error);
}

TYPED_TEST(GraphLoaderTest, SaveAndLoadGraph) {
    std::vector<std::vector<uint8_t>> matrix1 = {{0, 1, 2}, {0, 0, 1}, {1, 0, 0}};
    std::vector<std::vector<uint8_t>> matrix2 = {{0, 1}, {1, 0}};

    Multigraph<TypeParam> g1(std::move(matrix1));
    Multigraph<TypeParam> g2(std::move(matrix2));

    std::vector<std::tuple<TypeParam, TypeParam, uint8_t>> extension;
    extension.emplace_back(0, 1, 2);

    GraphLoader<TypeParam>::saveToFile(g1, g2, extension, 1, this->testFilePath);

    EXPECT_TRUE(std::filesystem::exists(this->testFilePath));

    auto [loaded1, loaded2] = GraphLoader<TypeParam>::loadFromFile(this->testFilePath);

    // Note: loadFromFile returns smaller graph first
    if (loaded1.getVertexCount() > loaded2.getVertexCount()) {
        std::swap(loaded1, loaded2);
    }

    EXPECT_EQ(loaded2.getVertexCount(), 3);
    EXPECT_EQ(loaded1.getVertexCount(), 2);
}

TYPED_TEST(GraphLoaderTest, LoadGraphPreservesEdgeCounts) {
    this->createTestFile(R"(2
0 5
2 0
3
0 1 0
0 0 3
0 0 0
)");

    auto [pattern, target] = GraphLoader<TypeParam>::loadFromFile(this->testFilePath);

    // Smaller graph (size 2) should be pattern
    EXPECT_EQ(pattern.getVertexCount(), 2);
    EXPECT_EQ(pattern.getEdges(0, 1), 5);
    EXPECT_EQ(pattern.getEdges(1, 0), 2);

    // Larger graph (size 3) should be target
    EXPECT_EQ(target.getVertexCount(), 3);
    EXPECT_EQ(target.getEdges(0, 1), 1);
    EXPECT_EQ(target.getEdges(1, 2), 3);
}

// Main function
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
