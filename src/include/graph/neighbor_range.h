#pragma once

#include "neighbor_iterator.h"
#include <vector>

namespace Subgraphs {

class OutNeighborRange {
  public:
    OutNeighborRange(const std::vector<uint8_t>& neighbors);

    OutNeighborIterator begin() const;
    OutNeighborIterator end() const;

  private:
    const std::vector<uint8_t>* neighbors;
};

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
