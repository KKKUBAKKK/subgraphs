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

// Template implementation

template <typename IndexType>
PermutationIterator<IndexType>::PermutationIterator(IndexType n, bool end)
    : permutation(static_cast<size_t>(n)), n(n), isEnd(end) {
    if (!isEnd && n > 0) {
        std::iota(permutation.begin(), permutation.end(), IndexType(0));
    }
}

template <typename IndexType>
const std::vector<IndexType>& PermutationIterator<IndexType>::operator*() const {
    return permutation;
}

template <typename IndexType>
PermutationIterator<IndexType>& PermutationIterator<IndexType>::operator++() {
    if (isEnd) {
        return *this;
    }

    if (!std::next_permutation(permutation.begin(), permutation.end())) {
        isEnd = true;
    }

    return *this;
}

template <typename IndexType>
bool PermutationIterator<IndexType>::operator==(const PermutationIterator& other) const {
    if (isEnd && other.isEnd) {
        return true;
    }
    if (isEnd != other.isEnd) {
        return false;
    }
    return permutation == other.permutation;
}

template <typename IndexType>
bool PermutationIterator<IndexType>::operator!=(const PermutationIterator& other) const {
    return !(*this == other);
}

template <typename IndexType> PermutationRange<IndexType>::PermutationRange(IndexType n) : n(n) {
}

template <typename IndexType>
PermutationIterator<IndexType> PermutationRange<IndexType>::begin() const {
    return PermutationIterator<IndexType>(n, false);
}

template <typename IndexType>
PermutationIterator<IndexType> PermutationRange<IndexType>::end() const {
    return PermutationIterator<IndexType>(n, true);
}

// Type aliases for backward compatibility
using PermutationIteratorInt64 = PermutationIterator<int64_t>;
using PermutationRangeInt64 = PermutationRange<int64_t>;

} // namespace Subgraphs
