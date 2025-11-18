#pragma once

#include "../graph/edge.h"
#include "../graph/multigraph.h"
#include "../graph/sequence_iterator.h"
#include <numeric>

namespace Subgraphs {

template <typename IndexType = int64_t> class SubgraphAlgorithm {
  public:
    static std::vector<Edge<IndexType>> run(int n, Multigraph<IndexType>& P,
                                            Multigraph<IndexType>& G);

  private:
    static std::vector<std::vector<std::vector<Edge<IndexType>>>>
    getAllMissingEdges(Multigraph<IndexType>& P, Multigraph<IndexType>& G);

    static std::vector<Edge<IndexType>> findMinimalExtension(
        int n, Multigraph<IndexType>& P, Multigraph<IndexType>& G,
        const std::vector<std::vector<std::vector<Edge<IndexType>>>>& allMissingEdges);
};

} // namespace Subgraphs

#include "subgraph_algorithm.inl"
