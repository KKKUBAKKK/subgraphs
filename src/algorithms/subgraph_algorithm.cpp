#include "algorithms/subgraph_algorithm.h"
#include "graph/sequence_iterator.h"
#include <numeric>

namespace Subgraphs {

std::vector<std::vector<std::vector<Edge>>> SubgraphAlgorithm::getAllMissingEdges(Multigraph& P,
                                                                                  Multigraph& G) {
    int64_t numPerms = P.permutationsCount();
    int64_t numCombs = G.combinationsCount(P.getVertexCount());
    std::vector<std::vector<std::vector<Edge>>> missingEdges(
        numPerms, std::vector<std::vector<Edge>>(numCombs));

    int64_t permIdx = 0;
    for (const auto& perm : P.permutations()) {
        int64_t combIdx = 0;
        for (const auto& comb : G.combinations(P.getVertexCount())) {
            for (int64_t i = 0; i < P.getVertexCount(); ++i) {
                for (int64_t j = 0; j < P.getVertexCount(); ++j) {
                    uint64_t pu = perm[i];
                    uint64_t pv = perm[j];
                    uint64_t gu = comb[i];
                    uint64_t gv = comb[j];

                    uint8_t pEdges = P.getEdges(pu, pv);
                    uint8_t gEdges = G.getEdges(gu, gv);

                    if (pEdges > gEdges) {
                        uint8_t delta = pEdges - gEdges;
                        missingEdges[permIdx][combIdx].emplace_back(gu, gv, delta);
                    }
                }
            }
            ++combIdx;
        }
        ++permIdx;
    }

    return missingEdges;
};

std::vector<Edge> SubgraphAlgorithm::findMinimalExtension(
    int n, Multigraph& P, Multigraph& G,
    const std::vector<std::vector<std::vector<Edge>>>& allMissingEdges) {
    std::vector<Edge> minimalExtension;
    int64_t minSize = INT64_MAX;

    for (auto combs : CombinationRange(G.combinationsCount(P.getVertexCount()), n)) {
        for (auto perms : SequenceRange(P.permutationsCount(), n)) {
            std::unordered_map<Edge, uint64_t> edgeFreqMap{};

            for (int i = 0; i < n; ++i) {
                std::unordered_map<Edge, uint64_t> localFreq{};
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

std::vector<std::vector<std::vector<Edge>>> SubgraphAlgorithm::run(int n, Multigraph& P,
                                                                   Multigraph& G) {
    auto allMissingEdges = getAllMissingEdges(P, G);

    std::vector<std::vector<std::vector<Edge>>> result;
    auto extension = findMinimalExtension(n, P, G, allMissingEdges);

    result.push_back({extension});
    return result;
}
} // namespace Subgraphs
