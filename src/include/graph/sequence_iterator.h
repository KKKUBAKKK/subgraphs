#pragma once

#include <cstdint>
#include <vector>

namespace Subgraphs {

template <typename IndexType = uint64_t> class SequenceIterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::vector<IndexType>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    SequenceIterator(IndexType maxValue, IndexType length, bool isEnd = false);

    reference operator*() const;
    pointer operator->() const;

    SequenceIterator& operator++();
    SequenceIterator operator++(int);

    bool operator==(const SequenceIterator& other) const;
    bool operator!=(const SequenceIterator& other) const;

  private:
    IndexType maxValue;
    IndexType length;
    std::vector<IndexType> current;
    bool isEnd;

    void generateNext();
};

template <typename IndexType = uint64_t> class SequenceRange {
  public:
    SequenceRange(IndexType maxValue, IndexType length);

    SequenceIterator<IndexType> begin() const;
    SequenceIterator<IndexType> end() const;

  private:
    IndexType maxValue;
    IndexType length;
};

} // namespace Subgraphs

#include "sequence_iterator.inl"
