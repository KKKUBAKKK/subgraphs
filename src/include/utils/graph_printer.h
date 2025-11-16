#pragma once

#include <filesystem>
#include <iostream>
#include <vector>

#include "../algorithms/subgraph_algorithm.h"
#include "../graph/multigraph.h"

namespace Subgraphs {

template <typename IndexType = int64_t> class GraphPrinter {
  public:
    static void printAdjacencyMatrix(const std::vector<std::vector<uint8_t>>& adjMatrix);

    static void printGraph(const Multigraph<IndexType>& graph, const std::string& title);
    static void printExtension(const std::vector<Edge<IndexType>>& extension);
    static void printResults(const Multigraph<IndexType>& patternGraph,
                             const Multigraph<IndexType>& targetGraph,
                             const std::vector<Edge<IndexType>>& extension);

    // Deprecated: use printExtension instead
    static void printToTerminal(const std::vector<Edge<IndexType>>& extension);
};

// Template implementation

template <typename IndexType>
void GraphPrinter<IndexType>::printAdjacencyMatrix(
    const std::vector<std::vector<uint8_t>>& adjMatrix) {
    int n = static_cast<int>(adjMatrix.size());
    std::cout << n << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << static_cast<int>(adjMatrix[i][j]);
            if (j + 1 < n)
                std::cout << " ";
        }
        std::cout << "\n";
    }
}

template <typename IndexType>
void GraphPrinter<IndexType>::printGraph(const Multigraph<IndexType>& graph,
                                         const std::string& title) {
    std::cout << "=== " << title << " ===" << std::endl;
    std::cout << "Vertices: " << graph.getVertexCount() << std::endl;
    std::cout << "Edges: " << graph.getEdgeCount() << std::endl;
    std::cout << "Adjacency Matrix:" << std::endl;
    graph.printAdjacencyMatrix();
}

template <typename IndexType>
void GraphPrinter<IndexType>::printExtension(const std::vector<Edge<IndexType>>& extension) {
    std::cout << "\n=== Graph Extension (edges to be added) ===\n";
    if (extension.empty()) {
        std::cout << "No edges need to be added (pattern already exists in target graph).\n";
        return;
    }

    int totalCost = 0;
    for (const auto& [u, v, w] : extension) {
        std::cout << "  Edge: " << u << " -> " << v << " (add " << static_cast<int>(w)
                  << " edge(s))\n";
        totalCost += w;
    }
    std::cout << "Total extension cost: " << totalCost << " edge(s)\n";
}

template <typename IndexType>
void GraphPrinter<IndexType>::printResults(const Multigraph<IndexType>& patternGraph,
                                           const Multigraph<IndexType>& targetGraph,
                                           const std::vector<Edge<IndexType>>& extension) {
    // Print pattern graph
    printGraph(patternGraph, "Pattern Graph (P)");

    // Print target graph
    std::cout << std::endl;
    printGraph(targetGraph, "Target Graph (G)");

    // Print extension
    printExtension(extension);

    // Create and print modified graph
    Multigraph<IndexType> modifiedGraph(targetGraph);
    for (const auto& [source, dest, count] : extension) {
        modifiedGraph.addEdges(source, dest, count);
    }

    std::cout << std::endl;
    printGraph(modifiedGraph, "Modified Target Graph (after adding extension)");
}

template <typename IndexType>
void GraphPrinter<IndexType>::printToTerminal(const std::vector<Edge<IndexType>>& extension) {
    printExtension(extension);
}

// Type alias for backward compatibility
using GraphPrinterInt64 = GraphPrinter<int64_t>;

} // namespace Subgraphs
