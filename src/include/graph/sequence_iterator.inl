
namespace Subgraphs {

template <typename IndexType>
SequenceIterator<IndexType>::SequenceIterator(IndexType maxValue, IndexType length, bool isEnd)
    : maxValue(maxValue), length(length), current(static_cast<size_t>(length), 0), isEnd(isEnd) {
    if (isEnd || maxValue == 0 || length == 0) {
        this->isEnd = true;
    }
}

template <typename IndexType>
typename SequenceIterator<IndexType>::reference SequenceIterator<IndexType>::operator*() const {
    return current;
}

template <typename IndexType>
typename SequenceIterator<IndexType>::pointer SequenceIterator<IndexType>::operator->() const {
    return &current;
}

template <typename IndexType>
SequenceIterator<IndexType>& SequenceIterator<IndexType>::operator++() {
    generateNext();
    return *this;
}

template <typename IndexType>
SequenceIterator<IndexType> SequenceIterator<IndexType>::operator++(int) {
    SequenceIterator tmp = *this;
    ++(*this);
    return tmp;
}

template <typename IndexType>
bool SequenceIterator<IndexType>::operator==(const SequenceIterator& other) const {
    if (isEnd && other.isEnd) {
        return true;
    }
    if (isEnd != other.isEnd) {
        return false;
    }
    return current == other.current && maxValue == other.maxValue && length == other.length;
}

template <typename IndexType>
bool SequenceIterator<IndexType>::operator!=(const SequenceIterator& other) const {
    return !(*this == other);
}

template <typename IndexType> void SequenceIterator<IndexType>::generateNext() {
    if (isEnd) {
        return;
    }

    int64_t pos = static_cast<int64_t>(length) - 1;
    while (pos >= 0) {
        ++current[pos];
        if (current[pos] < maxValue) {
            return;
        }
        current[pos] = 0;
        --pos;
    }

    isEnd = true;
}

template <typename IndexType>
SequenceRange<IndexType>::SequenceRange(IndexType maxValue, IndexType length)
    : maxValue(maxValue), length(length) {
}

template <typename IndexType> SequenceIterator<IndexType> SequenceRange<IndexType>::begin() const {
    return SequenceIterator<IndexType>(maxValue, length, false);
}

template <typename IndexType> SequenceIterator<IndexType> SequenceRange<IndexType>::end() const {
    return SequenceIterator<IndexType>(maxValue, length, true);
}

} // namespace Subgraphs
