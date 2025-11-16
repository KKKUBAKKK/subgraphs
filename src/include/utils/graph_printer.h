#pragma once

#include <filesystem>

#include "../algorithms/subgraph_algorithm.h"
#include "../graph/multigraph.h"

namespace Subgraphs {

class GraphPrinter {
  public:
    static void printAdjacencyMatrix(const std::vector<std::vector<uint8_t>>& adjMatrix);

    static void printGraph(const Multigraph& graph, const std::string& title);
    static void printExtension(const std::vector<Edge>& extension);
    static void printResults(const Multigraph& patternGraph, const Multigraph& targetGraph,
                             const std::vector<Edge>& extension);

    // Deprecated: use printExtension instead
    static void printToTerminal(const std::vector<Edge>& extension);
};

} // namespace Subgraphs
