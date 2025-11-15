#pragma once

#include "neighbor_iterator.h"
#include <vector>

namespace Subgraphs {

// Range wrapper for out-neighbors (iterates over a single row)
class NeighborRange {
  public:
    NeighborRange(const std::vector<uint8_t>& neighbors);

    NeighborIterator begin() const;
    NeighborIterator end() const;

  private:
    const std::vector<uint8_t>* neighbors;
};

// Alias for consistency
using OutNeighborRange = NeighborRange;

// Range wrapper for in-neighbors (iterates over a column)
class InNeighborRange {
  public:
    InNeighborRange(const std::vector<std::vector<uint8_t>>& adjMatrix, int64_t vertex);

    InNeighborIterator begin() const;
    InNeighborIterator end() const;

  private:
    const std::vector<std::vector<uint8_t>>* adjMatrix;
    int64_t vertex;
};

} // namespace Subgraphs
