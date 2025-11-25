#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

namespace Subgraphs {

template <typename IndexType = int64_t> struct Edge {
    IndexType source;
    IndexType destination;
    uint8_t count;

    Edge() = default;
    Edge(IndexType s, IndexType d, uint8_t c) : source(s), destination(d), count(c) {}

    bool operator==(const Edge& other) const {
        return source == other.source && destination == other.destination && count == other.count;
    }
};

} // namespace Subgraphs

namespace std {
template <typename IndexType> struct hash<Subgraphs::Edge<IndexType>> {
    std::size_t operator()(const Subgraphs::Edge<IndexType>& e) const noexcept {
        std::size_t h1 = std::hash<IndexType>{}(e.source);
        std::size_t h2 = std::hash<IndexType>{}(e.destination);
        return h1 ^ (h2 << 1);
    }
};

// Hash specialization for std::pair (needed for unordered_map keys)
template <typename T1, typename T2>
struct hash<std::pair<T1, T2>> {
    std::size_t operator()(const std::pair<T1, T2>& p) const noexcept {
        std::size_t h1 = std::hash<T1>{}(p.first);
        std::size_t h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};
} // namespace std
