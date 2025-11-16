#pragma once

#include "../graph/multigraph.h"

namespace Subgraphs {

struct Edge {
    int64_t source;
    int64_t destination;
    uint8_t count;
};

class SubgraphAlgorithm {
  public:
    static std::vector<std::vector<std::vector<Edge>>> run(int n, Multigraph& P, Multigraph& G);

  private:
    static std::vector<std::vector<std::vector<Edge>>> getAllMissingEdges(Multigraph& P,
                                                                          Multigraph& G);
    static std::vector<Edge>
    findMinimalExtension(int n, Multigraph& P, Multigraph& G,
                         const std::vector<std::vector<std::vector<Edge>>>& allMissingEdges);
};

} // namespace Subgraphs
