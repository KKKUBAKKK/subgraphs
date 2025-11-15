#pragma once

#include <iterator>
#include <vector>

namespace Subgraphs {

class OutNeighborIterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::pair<int64_t, uint8_t>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    OutNeighborIterator(const std::vector<uint8_t>& neighbors, int64_t position);

    value_type operator*() const;
    OutNeighborIterator& operator++();
    bool operator==(const OutNeighborIterator& other) const;
    bool operator!=(const OutNeighborIterator& other) const;

  private:
    void skipZeros();

    const std::vector<uint8_t>* neighbors;
    int64_t position;
};

class InNeighborIterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::pair<int64_t, uint8_t>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    InNeighborIterator(const std::vector<std::vector<uint8_t>>& adjMatrix, int64_t vertex,
                       int64_t position);

    value_type operator*() const;
    InNeighborIterator& operator++();
    bool operator==(const InNeighborIterator& other) const;
    bool operator!=(const InNeighborIterator& other) const;

  private:
    void skipZeros();

    const std::vector<std::vector<uint8_t>>* adjMatrix;
    int64_t vertex;
    int64_t position;
};

} // namespace Subgraphs
