#pragma once

#include "../graph/multigraph.h"
#include "../graph/sequence_iterator.h"
#include <numeric>
#include <unordered_map>

namespace Subgraphs {

template <typename IndexType = int64_t> struct Edge {
    IndexType source;
    IndexType destination;
    uint8_t count;

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
        std::size_t h3 = std::hash<uint8_t>{}(e.count);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
} // namespace std

namespace Subgraphs {

template <typename IndexType = int64_t> class SubgraphAlgorithm {
  public:
    static std::vector<std::vector<std::vector<Edge<IndexType>>>>
    run(int n, Multigraph<IndexType>& P, Multigraph<IndexType>& G);

  private:
    static std::vector<std::vector<std::vector<Edge<IndexType>>>>
    getAllMissingEdges(Multigraph<IndexType>& P, Multigraph<IndexType>& G);

    static std::vector<Edge<IndexType>> findMinimalExtension(
        int n, Multigraph<IndexType>& P, Multigraph<IndexType>& G,
        const std::vector<std::vector<std::vector<Edge<IndexType>>>>& allMissingEdges);
};

// Template implementation

template <typename IndexType>
std::vector<std::vector<std::vector<Edge<IndexType>>>>
SubgraphAlgorithm<IndexType>::getAllMissingEdges(Multigraph<IndexType>& P,
                                                 Multigraph<IndexType>& G) {
    IndexType numPerms = P.permutationsCount();
    IndexType numCombs = G.combinationsCount(P.getVertexCount());
    std::vector<std::vector<std::vector<Edge<IndexType>>>> missingEdges(
        static_cast<size_t>(numPerms),
        std::vector<std::vector<Edge<IndexType>>>(static_cast<size_t>(numCombs)));

    IndexType permIdx = 0;
    for (const auto& perm : P.permutations()) {
        IndexType combIdx = 0;
        for (const auto& comb : G.combinations(P.getVertexCount())) {
            for (IndexType i = 0; i < P.getVertexCount(); ++i) {
                for (IndexType j = 0; j < P.getVertexCount(); ++j) {
                    uint64_t pu = perm[i];
                    uint64_t pv = perm[j];
                    uint64_t gu = comb[i];
                    uint64_t gv = comb[j];

                    uint8_t pEdges = P.getEdges(pu, pv);
                    uint8_t gEdges = G.getEdges(gu, gv);

                    if (pEdges > gEdges) {
                        uint8_t delta = pEdges - gEdges;
                        missingEdges[permIdx][combIdx].emplace_back(
                            static_cast<IndexType>(gu), static_cast<IndexType>(gv), delta);
                    }
                }
            }
            ++combIdx;
        }
        ++permIdx;
    }

    return missingEdges;
}

template <typename IndexType>
std::vector<Edge<IndexType>> SubgraphAlgorithm<IndexType>::findMinimalExtension(
    int n, Multigraph<IndexType>& P, Multigraph<IndexType>& G,
    const std::vector<std::vector<std::vector<Edge<IndexType>>>>& allMissingEdges) {
    std::vector<Edge<IndexType>> minimalExtension;
    int64_t minSize = INT64_MAX;

    for (auto combs : CombinationRange<IndexType>(G.combinationsCount(P.getVertexCount()), n)) {
        for (auto perms : SequenceRange<IndexType>(P.permutationsCount(), n)) {
            std::unordered_map<Edge<IndexType>, uint64_t> edgeFreqMap{};

            for (int i = 0; i < n; ++i) {
                std::unordered_map<Edge<IndexType>, uint64_t> localFreq{};
                for (const auto& edge : allMissingEdges[perms[i]][combs[i]]) {
                    localFreq[edge] += edge.count;
                }

                for (const auto& [edge, freq] : localFreq) {
                    edgeFreqMap[edge] = std::max(edgeFreqMap[edge], freq);
                }
            }

            uint64_t currentSize = 0;
            for (const auto& [edge, freq] : edgeFreqMap) {
                currentSize += freq;
            }

            if (currentSize < minSize) {
                minSize = currentSize;
                minimalExtension.clear();
                for (const auto& [edge, count] : edgeFreqMap) {
                    minimalExtension.emplace_back(edge.source, edge.destination, count);
                }
            }
        }
    }

    return minimalExtension;
}

template <typename IndexType>
std::vector<std::vector<std::vector<Edge<IndexType>>>>
SubgraphAlgorithm<IndexType>::run(int n, Multigraph<IndexType>& P, Multigraph<IndexType>& G) {
    auto allMissingEdges = getAllMissingEdges(P, G);

    std::vector<std::vector<std::vector<Edge<IndexType>>>> result;
    auto extension = findMinimalExtension(n, P, G, allMissingEdges);

    result.push_back({extension});
    return result;
}

// Type aliases for backward compatibility
using EdgeInt64 = Edge<int64_t>;
using SubgraphAlgorithmInt64 = SubgraphAlgorithm<int64_t>;

} // namespace Subgraphs
