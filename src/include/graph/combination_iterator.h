#pragma once

#include <cstdint>
#include <vector>

namespace Subgraphs {

class CombinationIterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::vector<int64_t>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    CombinationIterator(int64_t n, int64_t k, bool end = false);

    const std::vector<int64_t>& operator*() const;
    CombinationIterator& operator++();
    bool operator==(const CombinationIterator& other) const;
    bool operator!=(const CombinationIterator& other) const;

  private:
    bool nextCombination();

    std::vector<int64_t> combination;
    int64_t n;
    int64_t k;
    bool isEnd;
};

class CombinationRange {
  public:
    CombinationRange(int64_t n, int64_t k);

    CombinationIterator begin() const;
    CombinationIterator end() const;

  private:
    int64_t n;
    int64_t k;
};

} // namespace Subgraphs
