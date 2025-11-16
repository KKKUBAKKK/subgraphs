#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../graph/multigraph.h"

namespace Subgraphs {

template <typename IndexType = int64_t> class GraphLoader {
  public:
    GraphLoader() = delete;

    static std::pair<Multigraph<IndexType>, Multigraph<IndexType>>
    loadFromFile(const std::filesystem::path& filePath);

    static void saveToFile(const Multigraph<IndexType>& g1, const Multigraph<IndexType>& g2,
                           const std::vector<std::tuple<IndexType, IndexType, uint8_t>>& extension,
                           int subgraphsCount, const std::filesystem::path& filePath);

  private:
    static std::vector<std::vector<uint8_t>>
    loadAdjacencyMatrix(const std::filesystem::path& filePath);
};

} // namespace Subgraphs

#include "graph_loader.inl"
