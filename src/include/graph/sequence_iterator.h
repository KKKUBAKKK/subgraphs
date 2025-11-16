#pragma once

#include <cstdint>
#include <vector>

namespace Subgraphs {

class SequenceIterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::vector<uint64_t>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    SequenceIterator(uint64_t maxValue, uint64_t length, bool isEnd = false);

    reference operator*() const;
    pointer operator->() const;

    SequenceIterator& operator++();
    SequenceIterator operator++(int);

    bool operator==(const SequenceIterator& other) const;
    bool operator!=(const SequenceIterator& other) const;

  private:
    uint64_t maxValue;
    uint64_t length;
    std::vector<uint64_t> current;
    bool isEnd;

    void generateNext();
};

class SequenceRange {
  public:
    SequenceRange(uint64_t maxValue, uint64_t length);

    SequenceIterator begin() const;
    SequenceIterator end() const;

  private:
    uint64_t maxValue;
    uint64_t length;
};

} // namespace Subgraphs
