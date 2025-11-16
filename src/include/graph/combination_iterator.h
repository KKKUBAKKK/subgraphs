#pragma once

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

// Template implementation

template <typename IndexType>
CombinationIterator<IndexType>::CombinationIterator(IndexType n, IndexType k, bool end)
    : combination(static_cast<size_t>(k)), n(n), k(k), isEnd(end) {
    if (!isEnd && k > 0 && k <= n) {
        for (IndexType i = 0; i < k; ++i) {
            combination[i] = i;
        }
    } else if (k > n || k <= 0) {
        isEnd = true;
    }
}

template <typename IndexType>
const std::vector<IndexType>& CombinationIterator<IndexType>::operator*() const {
    return combination;
}

template <typename IndexType> bool CombinationIterator<IndexType>::nextCombination() {
    IndexType i = k;
    while (i > 0 && combination[i - 1] == n - k + i - 1) {
        --i;
    }

    if (i == 0) {
        return false;
    }

    --i;
    ++combination[i];

    for (IndexType j = i + 1; j < k; ++j) {
        combination[j] = combination[j - 1] + 1;
    }

    return true;
}

template <typename IndexType>
CombinationIterator<IndexType>& CombinationIterator<IndexType>::operator++() {
    if (isEnd) {
        return *this;
    }

    if (!nextCombination()) {
        isEnd = true;
    }

    return *this;
}

template <typename IndexType>
bool CombinationIterator<IndexType>::operator==(const CombinationIterator& other) const {
    if (isEnd && other.isEnd) {
        return true;
    }
    if (isEnd != other.isEnd) {
        return false;
    }
    return combination == other.combination && n == other.n && k == other.k;
}

template <typename IndexType>
bool CombinationIterator<IndexType>::operator!=(const CombinationIterator& other) const {
    return !(*this == other);
}

template <typename IndexType>
CombinationRange<IndexType>::CombinationRange(IndexType n, IndexType k) : n(n), k(k) {
}

template <typename IndexType>
CombinationIterator<IndexType> CombinationRange<IndexType>::begin() const {
    return CombinationIterator<IndexType>(n, k, false);
}

template <typename IndexType>
CombinationIterator<IndexType> CombinationRange<IndexType>::end() const {
    return CombinationIterator<IndexType>(n, k, true);
}

// Type aliases for backward compatibility
using CombinationIteratorInt64 = CombinationIterator<int64_t>;
using CombinationRangeInt64 = CombinationRange<int64_t>;

} // namespace Subgraphs
