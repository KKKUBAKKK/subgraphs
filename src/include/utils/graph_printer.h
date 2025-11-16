#pragma once

#include <filesystem>

#include "../algorithms/subgraph_algorithm.h"
#include "../graph/multigraph.h"

namespace Subgraphs {

class GraphPrinter {
  public:
    static void printAdjacencyMatrix(const std::vector<std::vector<uint8_t>>& adjMatrix);
    static void printToTerminal( // const std::vector<std::vector<uint8_t>>& adjMatrix,
        const std::vector<Edge>& extension);
    // static void printToFile(const std::vector<std::vector<uint8_t>>& adjMatrix,
    //                         const std::filesystem::path& filePath);
};

} // namespace Subgraphs
