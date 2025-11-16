#pragma once

#include <cstdint>
#include <vector>

namespace Subgraphs {

class PermutationIterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::vector<int64_t>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    PermutationIterator(int64_t n, bool end = false);

    const std::vector<int64_t>& operator*() const;
    PermutationIterator& operator++();
    bool operator==(const PermutationIterator& other) const;
    bool operator!=(const PermutationIterator& other) const;

  private:
    std::vector<int64_t> permutation;
    int64_t n;
    bool isEnd;
};

class PermutationRange {
  public:
    explicit PermutationRange(int64_t n);

    PermutationIterator begin() const;
    PermutationIterator end() const;

  private:
    int64_t n;
};

} // namespace Subgraphs
