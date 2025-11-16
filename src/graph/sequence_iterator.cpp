#include "graph/sequence_iterator.h"

namespace Subgraphs {

SequenceIterator::SequenceIterator(uint64_t maxValue, uint64_t length, bool isEnd)
    : maxValue(maxValue), length(length), current(length, 0), isEnd(isEnd) {
    if (isEnd || maxValue == 0 || length == 0) {
        this->isEnd = true;
    }
}

SequenceIterator::reference SequenceIterator::operator*() const {
    return current;
}

SequenceIterator::pointer SequenceIterator::operator->() const {
    return &current;
}

SequenceIterator& SequenceIterator::operator++() {
    generateNext();
    return *this;
}

SequenceIterator SequenceIterator::operator++(int) {
    SequenceIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool SequenceIterator::operator==(const SequenceIterator& other) const {
    if (isEnd && other.isEnd) {
        return true;
    }
    if (isEnd != other.isEnd) {
        return false;
    }
    return current == other.current && maxValue == other.maxValue && length == other.length;
}

bool SequenceIterator::operator!=(const SequenceIterator& other) const {
    return !(*this == other);
}

void SequenceIterator::generateNext() {
    if (isEnd) {
        return;
    }

    // Increment like a base-maxValue counter from right to left
    int64_t pos = static_cast<int64_t>(length) - 1;
    while (pos >= 0) {
        ++current[pos];
        if (current[pos] < maxValue) {
            return; // Successfully incremented
        }
        // Overflow at this position, reset and carry to next
        current[pos] = 0;
        --pos;
    }

    // If we've exhausted all positions, we're done
    isEnd = true;
}

SequenceRange::SequenceRange(uint64_t maxValue, uint64_t length)
    : maxValue(maxValue), length(length) {
}

SequenceIterator SequenceRange::begin() const {
    return SequenceIterator(maxValue, length, false);
}

SequenceIterator SequenceRange::end() const {
    return SequenceIterator(maxValue, length, true);
}

} // namespace Subgraphs
