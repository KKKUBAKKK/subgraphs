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

void GraphPrinter::printToTerminal(const std::vector<Edge>& extension) {
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

// void GraphPrinter::printToFile(const std::vector<std::vector<uint8_t>>& adjMatrix,
//                                const std::filesystem::path& filePath) {
//     std::ofstream out(filePath);
//     if (!out.is_open()) {
//         throw std::runtime_error("Could not open file for writing: " + filePath.string());
//     }

//     printAdjacencyMatrix(adjMatrix, out);
// }

} // namespace Subgraphs
