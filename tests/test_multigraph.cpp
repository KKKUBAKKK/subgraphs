// #include "graph/multigraph.h"
// #include <cassert>
// #include <iostream>
// #include <vector>

// void testConstructorEmpty() {
//     std::cout << "Testing: Constructor with vertex count...\n";
//     Subgraphs::Multigraph graph(5);
//     assert(graph.getVertexCount() == 5);
//     assert(graph.getEdgeCount() == 0);
//     std::cout << "✓ Passed\n\n";
// }

// void testConstructorFromMatrix() {
//     std::cout << "Testing: Constructor from adjacency matrix...\n";
//     std::vector<std::vector<uint8_t>> matrix = {{0, 1, 2}, {0, 0, 1}, {1, 0, 0}};
//     Subgraphs::Multigraph graph(std::move(matrix));
//     assert(graph.getVertexCount() == 3);
//     assert(graph.getEdgeCount() == 5); // 1 + 2 + 1 + 1 = 5
//     std::cout << "✓ Passed\n\n";
// }

// void testCopyConstructor() {
//     std::cout << "Testing: Copy constructor...\n";
//     Subgraphs::Multigraph graph1(3);
//     graph1.addEdges(0, 1, 2);
//     graph1.addEdges(1, 2, 1);

//     Subgraphs::Multigraph graph2(graph1);
//     assert(graph2.getVertexCount() == graph1.getVertexCount());
//     assert(graph2.getEdgeCount() == graph1.getEdgeCount());
//     assert(graph2.getEdges(0, 1) == 2);
//     assert(graph2.getEdges(1, 2) == 1);
//     std::cout << "✓ Passed\n\n";
// }

// void testAddEdges() {
//     std::cout << "Testing: addEdges()...\n";
//     Subgraphs::Multigraph graph(4);

//     graph.addEdges(0, 1, 1);
//     assert(graph.getEdges(0, 1) == 1);
//     assert(graph.getEdgeCount() == 1);

//     graph.addEdges(0, 1, 2);
//     assert(graph.getEdges(0, 1) == 3);
//     assert(graph.getEdgeCount() == 3);

//     graph.addEdges(2, 3, 5);
//     assert(graph.getEdges(2, 3) == 5);
//     assert(graph.getEdgeCount() == 8);
//     std::cout << "✓ Passed\n\n";
// }

// void testGetEdges() {
//     std::cout << "Testing: getEdges()...\n";
//     Subgraphs::Multigraph graph(3);

//     assert(graph.getEdges(0, 1) == 0);
//     graph.addEdges(0, 1, 3);
//     assert(graph.getEdges(0, 1) == 3);
//     assert(graph.getEdges(1, 0) == 0); // Directed graph
//     std::cout << "✓ Passed\n\n";
// }

// void testDegrees() {
//     std::cout << "Testing: getDegree(), getInDegree(), getOutDegree()...\n";
//     Subgraphs::Multigraph graph(4);

//     // Add edges: 0 -> 1 (weight 2), 1 -> 2 (weight 1), 2 -> 0 (weight 3)
//     graph.addEdges(0, 1, 2);
//     graph.addEdges(1, 2, 1);
//     graph.addEdges(2, 0, 3);

//     // Vertex 0: out-degree = 2, in-degree = 3
//     assert(graph.getOutDegree(0) == 2);
//     assert(graph.getInDegree(0) == 3);
//     auto deg0 = graph.getDegree(0);
//     assert(deg0.outDegree == 2);
//     assert(deg0.inDegree == 3);

//     // Vertex 1: out-degree = 1, in-degree = 2
//     assert(graph.getOutDegree(1) == 1);
//     assert(graph.getInDegree(1) == 2);

//     // Vertex 2: out-degree = 3, in-degree = 1
//     assert(graph.getOutDegree(2) == 3);
//     assert(graph.getInDegree(2) == 1);

//     // Vertex 3: out-degree = 0, in-degree = 0
//     assert(graph.getOutDegree(3) == 0);
//     assert(graph.getInDegree(3) == 0);

//     std::cout << "✓ Passed\n\n";
// }

// void testGetOutNeighbors() {
//     std::cout << "Testing: getOutNeighbors()...\n";
//     Subgraphs::Multigraph graph(4);

//     graph.addEdges(0, 1, 2);
//     graph.addEdges(0, 2, 1);
//     graph.addEdges(0, 3, 3);

//     auto neighbors = graph.getOutNeighbors(0);
//     assert(neighbors.size() == 3);
//     assert(neighbors[0].first == 1 && neighbors[0].second == 2);
//     assert(neighbors[1].first == 2 && neighbors[1].second == 1);
//     assert(neighbors[2].first == 3 && neighbors[2].second == 3);

//     auto noNeighbors = graph.getOutNeighbors(3);
//     assert(noNeighbors.empty());

//     std::cout << "✓ Passed\n\n";
// }

// void testGetInNeighbors() {
//     std::cout << "Testing: getInNeighbors()...\n";
//     Subgraphs::Multigraph graph(4);

//     graph.addEdges(0, 2, 2);
//     graph.addEdges(1, 2, 1);
//     graph.addEdges(3, 2, 3);

//     auto neighbors = graph.getInNeighbors(2);
//     assert(neighbors.size() == 3);
//     assert(neighbors[0].first == 0 && neighbors[0].second == 2);
//     assert(neighbors[1].first == 1 && neighbors[1].second == 1);
//     assert(neighbors[2].first == 3 && neighbors[2].second == 3);

//     auto noNeighbors = graph.getInNeighbors(0);
//     assert(noNeighbors.empty());

//     std::cout << "✓ Passed\n\n";
// }

// void testOutNeighborIterator() {
//     std::cout << "Testing: outNeighbors() iterator...\n";
//     Subgraphs::Multigraph graph(5);

//     graph.addEdges(0, 1, 1);
//     graph.addEdges(0, 3, 2);
//     graph.addEdges(0, 4, 1);

//     std::vector<std::pair<int64_t, uint8_t>> result;
//     for (auto [neighbor, weight] : graph.outNeighbors(0)) {
//         result.emplace_back(neighbor, weight);
//     }

//     assert(result.size() == 3);
//     assert(result[0].first == 1 && result[0].second == 1);
//     assert(result[1].first == 3 && result[1].second == 2);
//     assert(result[2].first == 4 && result[2].second == 1);

//     std::cout << "✓ Passed\n\n";
// }

// void testInNeighborIterator() {
//     std::cout << "Testing: inNeighbors() iterator...\n";
//     Subgraphs::Multigraph graph(5);

//     graph.addEdges(0, 2, 1);
//     graph.addEdges(1, 2, 3);
//     graph.addEdges(4, 2, 2);

//     std::vector<std::pair<int64_t, uint8_t>> result;
//     for (auto [neighbor, weight] : graph.inNeighbors(2)) {
//         result.emplace_back(neighbor, weight);
//     }

//     assert(result.size() == 3);
//     assert(result[0].first == 0 && result[0].second == 1);
//     assert(result[1].first == 1 && result[1].second == 3);
//     assert(result[2].first == 4 && result[2].second == 2);

//     std::cout << "✓ Passed\n\n";
// }

// void testEmptyIterators() {
//     std::cout << "Testing: Empty neighbor iterators...\n";
//     Subgraphs::Multigraph graph(3);

//     int count = 0;
//     for (auto [neighbor, weight] : graph.outNeighbors(0)) {
//         (void)neighbor;
//         (void)weight;
//         count++;
//     }
//     assert(count == 0);

//     count = 0;
//     for (auto [neighbor, weight] : graph.inNeighbors(1)) {
//         (void)neighbor;
//         (void)weight;
//         count++;
//     }
//     assert(count == 0);

//     std::cout << "✓ Passed\n\n";
// }

// void testMultipleEdges() {
//     std::cout << "Testing: Multiple edges (multigraph property)...\n";
//     Subgraphs::Multigraph graph(3);

//     graph.addEdges(0, 1, 1);
//     graph.addEdges(0, 1, 1);
//     graph.addEdges(0, 1, 3);

//     assert(graph.getEdges(0, 1) == 5);
//     assert(graph.getEdgeCount() == 5);
//     assert(graph.getOutDegree(0) == 5);
//     assert(graph.getInDegree(1) == 5);

//     std::cout << "✓ Passed\n\n";
// }

// void testSelfLoops() {
//     std::cout << "Testing: Self-loops...\n";
//     Subgraphs::Multigraph graph(3);

//     graph.addEdges(0, 0, 2);
//     graph.addEdges(1, 1, 1);

//     assert(graph.getEdges(0, 0) == 2);
//     assert(graph.getEdges(1, 1) == 1);
//     assert(graph.getEdgeCount() == 3);

//     // Self-loop contributes to both in and out degree
//     assert(graph.getOutDegree(0) == 2);
//     assert(graph.getInDegree(0) == 2);

//     std::cout << "✓ Passed\n\n";
// }

// void testLargeGraph() {
//     std::cout << "Testing: Larger graph...\n";
//     Subgraphs::Multigraph graph(100);

//     // Add some edges
//     for (int64_t i = 0; i < 99; ++i) {
//         graph.addEdges(i, i + 1, 1);
//     }

//     assert(graph.getVertexCount() == 100);
//     assert(graph.getEdgeCount() == 99);

//     // Check chain properties
//     assert(graph.getOutDegree(0) == 1);
//     assert(graph.getInDegree(0) == 0);
//     assert(graph.getOutDegree(99) == 0);
//     assert(graph.getInDegree(99) == 1);

//     for (int64_t i = 1; i < 99; ++i) {
//         assert(graph.getOutDegree(i) == 1);
//         assert(graph.getInDegree(i) == 1);
//     }

//     std::cout << "✓ Passed\n\n";
// }

// void testIteratorSkipsZeros() {
//     std::cout << "Testing: Iterator skips zero-weight edges...\n";
//     Subgraphs::Multigraph graph(10);

//     graph.addEdges(0, 2, 1);
//     graph.addEdges(0, 5, 1);
//     graph.addEdges(0, 9, 1);

//     int count = 0;
//     for (auto [neighbor, weight] : graph.outNeighbors(0)) {
//         (void)neighbor;
//         (void)weight;
//         count++;
//     }
//     assert(count == 3); // Should only iterate over non-zero edges

//     std::cout << "✓ Passed\n\n";
// }

// int main() {
//     std::cout << "========================================\n";
//     std::cout << "Running Multigraph Tests\n";
//     std::cout << "========================================\n\n";

//     try {
//         testConstructorEmpty();
//         testConstructorFromMatrix();
//         testCopyConstructor();
//         testAddEdges();
//         testGetEdges();
//         testDegrees();
//         testGetOutNeighbors();
//         testGetInNeighbors();
//         testOutNeighborIterator();
//         testInNeighborIterator();
//         testEmptyIterators();
//         testMultipleEdges();
//         testSelfLoops();
//         testLargeGraph();
//         testIteratorSkipsZeros();

//         std::cout << "========================================\n";
//         std::cout << "All tests passed! ✓\n";
//         std::cout << "========================================\n";
//         return 0;
//     } catch (const std::exception& e) {
//         std::cerr << "Test failed with exception: " << e.what() << "\n";
//         return 1;
//     }
// }
