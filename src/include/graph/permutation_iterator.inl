
namespace Subgraphs {

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

} // namespace Subgraphs
