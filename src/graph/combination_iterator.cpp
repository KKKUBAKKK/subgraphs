#include "graph/combination_iterator.h"

namespace Subgraphs {

CombinationIterator::CombinationIterator(int64_t n, int64_t k, bool end)
    : combination(k), n(n), k(k), isEnd(end) {
    if (!isEnd && k > 0 && k <= n) {
        for (int64_t i = 0; i < k; ++i) {
            combination[i] = i;
        }
    } else if (k > n || k <= 0) {
        isEnd = true;
    }
}

const std::vector<int64_t>& CombinationIterator::operator*() const {
    return combination;
}

bool CombinationIterator::nextCombination() {
    int64_t i = k - 1;
    while (i >= 0 && combination[i] == n - k + i) {
        --i;
    }

    if (i < 0) {
        return false;
    }

    ++combination[i];

    for (int64_t j = i + 1; j < k; ++j) {
        combination[j] = combination[j - 1] + 1;
    }

    return true;
}

CombinationIterator& CombinationIterator::operator++() {
    if (isEnd) {
        return *this;
    }

    if (!nextCombination()) {
        isEnd = true;
    }

    return *this;
}

bool CombinationIterator::operator==(const CombinationIterator& other) const {
    if (isEnd && other.isEnd) {
        return true;
    }
    if (isEnd != other.isEnd) {
        return false;
    }
    return combination == other.combination && n == other.n && k == other.k;
}

bool CombinationIterator::operator!=(const CombinationIterator& other) const {
    return !(*this == other);
}

CombinationRange::CombinationRange(int64_t n, int64_t k) : n(n), k(k) {
}

CombinationIterator CombinationRange::begin() const {
    return CombinationIterator(n, k, false);
}

CombinationIterator CombinationRange::end() const {
    return CombinationIterator(n, k, true);
}

} // namespace Subgraphs
