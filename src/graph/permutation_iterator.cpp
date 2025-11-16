#include "graph/permutation_iterator.h"
#include <algorithm>
#include <numeric>

namespace Subgraphs {

PermutationIterator::PermutationIterator(int64_t n, bool end) : permutation(n), n(n), isEnd(end) {
    if (!isEnd && n > 0) {
        std::iota(permutation.begin(), permutation.end(), 0);
    }
}

const std::vector<int64_t>& PermutationIterator::operator*() const {
    return permutation;
}

PermutationIterator& PermutationIterator::operator++() {
    if (isEnd) {
        return *this;
    }

    if (!std::next_permutation(permutation.begin(), permutation.end())) {
        isEnd = true;
    }

    return *this;
}

bool PermutationIterator::operator==(const PermutationIterator& other) const {
    if (isEnd && other.isEnd) {
        return true;
    }
    if (isEnd != other.isEnd) {
        return false;
    }
    return permutation == other.permutation;
}

bool PermutationIterator::operator!=(const PermutationIterator& other) const {
    return !(*this == other);
}

PermutationRange::PermutationRange(int64_t n) : n(n) {
}

PermutationIterator PermutationRange::begin() const {
    return PermutationIterator(n, false);
}

PermutationIterator PermutationRange::end() const {
    return PermutationIterator(n, true);
}

} // namespace Subgraphs
