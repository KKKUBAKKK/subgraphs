#pragma once

#include <vector>

#include "multigraph.h"

namespace Subgraphs {

class CombinationIterator {
  public:
    CombinationIterator(const Multigraph& graph, uint n, bool end = false);

    Multigraph operator*() const;
    CombinationIterator& operator++();
    bool operator!=(const CombinationIterator& other) const;

  private:
    const Multigraph& graph;
    std::vector<int> combination;
    uint n;
    bool isEnd;
};

} // namespace Subgraphs
