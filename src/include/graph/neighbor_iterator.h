#pragma once

#include <iterator>
#include <vector>

namespace Subgraphs {

class NeighborIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair<int64_t, uint8_t>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    NeighborIterator(const std::vector<uint8_t>& neighbors, int64_t position);

    value_type operator*() const;
    NeighborIterator& operator++();
    NeighborIterator operator++(int);
    bool operator==(const NeighborIterator& other) const;
    bool operator!=(const NeighborIterator& other) const;

  private:
    void skipZeros();

    const std::vector<uint8_t>* neighbors;
    int64_t position;
};

// Iterator for in-neighbors (iterates over columns)
class InNeighborIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair<int64_t, uint8_t>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    InNeighborIterator(const std::vector<std::vector<uint8_t>>& adjMatrix, int64_t vertex,
                       int64_t position);

    value_type operator*() const;
    InNeighborIterator& operator++();
    InNeighborIterator operator++(int);
    bool operator==(const InNeighborIterator& other) const;
    bool operator!=(const InNeighborIterator& other) const;

  private:
    void skipZeros();

    const std::vector<std::vector<uint8_t>>* adjMatrix;
    int64_t vertex;
    int64_t position;
};

} // namespace Subgraphs
