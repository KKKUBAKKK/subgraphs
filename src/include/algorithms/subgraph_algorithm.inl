
namespace Subgraphs {

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
                    IndexType pu = perm[i];
                    IndexType pv = perm[j];
                    IndexType gu = comb[i];
                    IndexType gv = comb[j];

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
}

template <typename IndexType>
std::vector<Edge<IndexType>> SubgraphAlgorithm<IndexType>::findMinimalExtension(
    int n, Multigraph<IndexType>& P, Multigraph<IndexType>& G,
    const std::vector<std::vector<std::vector<Edge<IndexType>>>>& allMissingEdges) {
    std::vector<Edge<IndexType>> minimalExtension;
    IndexType minSize = std::numeric_limits<IndexType>::max();

    std::unordered_map<Edge<IndexType>, uint64_t> edgeFreqMap;
    edgeFreqMap.reserve(n * P.getEdgeCount());

    for (auto combs : CombinationRange<IndexType>(G.combinationsCount(P.getVertexCount()), n)) {
        for (auto perms : SequenceRange<IndexType>(P.permutationsCount(), n)) {
            edgeFreqMap.clear();

            IndexType currentSize = 0;

            for (int i = 0; i < n; ++i) {
                for (const auto& edge : allMissingEdges[perms[i]][combs[i]]) {
                    auto it = edgeFreqMap.find(edge);
                    if (it == edgeFreqMap.end()) {
                        edgeFreqMap[edge] = edge.count;
                        currentSize += edge.count;
                    } else {
                        if (edge.count > it->second) {
                            currentSize += (edge.count - it->second);
                            it->second = edge.count;
                        }
                    }
                }
            }

            if (currentSize < minSize) {
                minSize = currentSize;
                minimalExtension.clear();
                minimalExtension.reserve(edgeFreqMap.size());
                for (const auto& [edge, count] : edgeFreqMap) {
                    minimalExtension.emplace_back(edge.source, edge.destination, count);
                }
            }
        }
    }

    return minimalExtension;
}

template <typename IndexType>
std::vector<Edge<IndexType>> SubgraphAlgorithm<IndexType>::run(int n, Multigraph<IndexType>& P,
                                                               Multigraph<IndexType>& G) {
    auto allMissingEdges = getAllMissingEdges(P, G);
    auto result = findMinimalExtension(n, P, G, allMissingEdges);
    return result;
}

} // namespace Subgraphs
