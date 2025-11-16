#pragma once

#include "../graph/multigraph.h"

namespace Subgraphs {

struct Edge {
    int64_t source;
    int64_t destination;
    uint8_t count;

    bool operator==(const Edge& other) const {
        return source == other.source && destination == other.destination && count == other.count;
    }
};

} // namespace Subgraphs

namespace std {
template <> struct hash<Subgraphs::Edge> {
    std::size_t operator()(const Subgraphs::Edge& e) const noexcept {
        std::size_t h1 = std::hash<int64_t>{}(e.source);
        std::size_t h2 = std::hash<int64_t>{}(e.destination);
        std::size_t h3 = std::hash<uint8_t>{}(e.count);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
} // namespace std

namespace Subgraphs {

class SubgraphAlgorithm {
  public:
    static std::vector<std::vector<std::vector<Edge>>> run(int n, Multigraph& P, Multigraph& G);

  private:
    static std::vector<std::vector<std::vector<Edge>>> getAllMissingEdges(Multigraph& P,
                                                                          Multigraph& G);
    static std::vector<Edge>
    findMinimalExtension(int n, Multigraph& P, Multigraph& G,
                         const std::vector<std::vector<std::vector<Edge>>>& allMissingEdges);
};

} // namespace Subgraphs
