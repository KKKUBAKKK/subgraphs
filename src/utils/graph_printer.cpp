#include "utils/graph_printer.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

namespace Subgraphs {

void GraphPrinter::printAdjacencyMatrix(const std::vector<std::vector<uint8_t>>& adjMatrix) {
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

void GraphPrinter::printGraph(const Multigraph& graph, const std::string& title) {
    std::cout << "=== " << title << " ===" << std::endl;
    std::cout << "Vertices: " << graph.getVertexCount() << std::endl;
    std::cout << "Edges: " << graph.getEdgeCount() << std::endl;
    std::cout << "Adjacency Matrix:" << std::endl;
    graph.printAdjacencyMatrix();
}

void GraphPrinter::printExtension(const std::vector<Edge>& extension) {
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

void GraphPrinter::printResults(const Multigraph& patternGraph, const Multigraph& targetGraph,
                                const std::vector<Edge>& extension) {
    // Print pattern graph
    printGraph(patternGraph, "Pattern Graph (P)");

    // Print target graph
    std::cout << std::endl;
    printGraph(targetGraph, "Target Graph (G)");

    // Print extension
    printExtension(extension);

    // Create and print modified graph
    Multigraph modifiedGraph(targetGraph);
    for (const auto& [source, dest, count] : extension) {
        modifiedGraph.addEdges(source, dest, count);
    }

    std::cout << std::endl;
    printGraph(modifiedGraph, "Modified Target Graph (after adding extension)");
}

void GraphPrinter::printToTerminal(const std::vector<Edge>& extension) {
    printExtension(extension);
}

} // namespace Subgraphs
