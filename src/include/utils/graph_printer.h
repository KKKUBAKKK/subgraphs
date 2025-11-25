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
};

} // namespace Subgraphs

#include "graph_printer.inl"
