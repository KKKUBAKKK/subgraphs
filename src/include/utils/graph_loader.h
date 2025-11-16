#pragma once

#include <filesystem>

#include "../graph/multigraph.h"

namespace Subgraphs {

class GraphLoader {
  public:
    GraphLoader() = delete;

    static std::pair<Multigraph, Multigraph> loadFromFile(const std::filesystem::path& filePath);
    static void saveToFile(const Multigraph& g1, const Multigraph& g2,
                           const std::vector<std::tuple<uint64_t, uint64_t, uint8_t>>& extension,
                           int subgraphsCount, const std::filesystem::path& filePath);

  private:
    static std::vector<std::vector<uint8_t>>
    loadAdjacencyMatrix(const std::filesystem::path& filePath);
};

} // namespace Subgraphs
