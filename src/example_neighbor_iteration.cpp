#include "graph/multigraph.h"
#include <iostream>

int main() {
    // Create a simple directed multigraph
    Subgraphs::Multigraph graph(4);

    // Add some edges
    graph.addEdges(0, 1, 2); // 0 -> 1 with weight 2
    graph.addEdges(0, 2, 1); // 0 -> 2 with weight 1
    graph.addEdges(1, 2, 1); // 1 -> 2 with weight 1
    graph.addEdges(2, 3, 3); // 2 -> 3 with weight 3
    graph.addEdges(3, 0, 1); // 3 -> 0 with weight 1

    std::cout << "Graph has " << graph.getVertexCount() << " vertices and " << graph.getEdgeCount()
              << " edges\n\n";

    // Iterate through out-neighbors (using range-based for loop)
    std::cout << "Out-neighbors of vertex 0:\n";
    for (auto [neighbor, weight] : graph.outNeighbors(0)) {
        std::cout << "  -> " << neighbor << " (weight: " << (int)weight << ")\n";
    }

    // Iterate through in-neighbors
    std::cout << "\nIn-neighbors of vertex 2:\n";
    for (auto [neighbor, weight] : graph.inNeighbors(2)) {
        std::cout << "  " << neighbor << " -> (weight: " << (int)weight << ")\n";
    }

    // Using neighbors() which is an alias for outNeighbors()
    std::cout << "\nNeighbors of vertex 2 (out-neighbors):\n";
    for (auto [neighbor, weight] : graph.neighbors(2)) {
        std::cout << "  -> " << neighbor << " (weight: " << (int)weight << ")\n";
    }

    // Manual iteration with explicit iterator
    std::cout << "\nManual iteration of out-neighbors of vertex 0:\n";
    auto range = graph.outNeighbors(0);
    for (auto it = range.begin(); it != range.end(); ++it) {
        auto [neighbor, weight] = *it;
        std::cout << "  -> " << neighbor << " (weight: " << (int)weight << ")\n";
    }

    return 0;
}
