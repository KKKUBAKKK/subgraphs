#pragma once

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace Subgraphs {

template <typename IndexType = int64_t> class PermutationIterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::vector<IndexType>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    PermutationIterator(IndexType n, bool end = false);

    const std::vector<IndexType>& operator*() const;
    PermutationIterator& operator++();
    bool operator==(const PermutationIterator& other) const;
    bool operator!=(const PermutationIterator& other) const;

  private:
    std::vector<IndexType> permutation;
    IndexType n;
    bool isEnd;
};

template <typename IndexType = int64_t> class PermutationRange {
  public:
    explicit PermutationRange(IndexType n);

    PermutationIterator<IndexType> begin() const;
    PermutationIterator<IndexType> end() const;

  private:
    IndexType n;
};

} // namespace Subgraphs

#include "permutation_iterator.inl"
