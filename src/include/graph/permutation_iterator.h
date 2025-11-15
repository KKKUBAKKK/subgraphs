#pragma once

#include <vector>

#include "multigraph.h"

namespace Subgraphs {

class PermutationIterator {
  public:
    PermutationIterator(const Multigraph& graph, bool end = false);

    Multigraph operator*() const;
    PermutationIterator& operator++();
    bool operator!=(const PermutationIterator& other) const;

  private:
    const Multigraph& graph;
    std::vector<int> permutation;
    bool isEnd;
};

} // namespace Subgraphs
