#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

namespace Subgraphs {

template <typename IndexType = int64_t> class CombinationIterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::vector<IndexType>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    CombinationIterator(IndexType n, IndexType k, bool end = false);

    const std::vector<IndexType>& operator*() const;
    CombinationIterator& operator++();
    bool operator==(const CombinationIterator& other) const;
    bool operator!=(const CombinationIterator& other) const;

  private:
    bool nextCombination();

    std::vector<IndexType> combination;
    IndexType n;
    IndexType k;
    bool isEnd;
};

template <typename IndexType = int64_t> class CombinationRange {
  public:
    CombinationRange(IndexType n, IndexType k);

    CombinationIterator<IndexType> begin() const;
    CombinationIterator<IndexType> end() const;

  private:
    IndexType n;
    IndexType k;
};

} // namespace Subgraphs

#include "combination_iterator.inl"
