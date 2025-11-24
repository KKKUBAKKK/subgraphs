
namespace Subgraphs {
// TODO(jakubkindracki): optimize combination and permutation iterations
template <typename IndexType>
std::vector<std::vector<std::vector<Edge<IndexType>>>>
SubgraphAlgorithm<IndexType>::getAllMissingEdges(Multigraph<IndexType>& P,
                                                 Multigraph<IndexType>& G) {
    const IndexType numPerms = P.permutationsCount();
    const IndexType numCombs = G.combinationsCount(P.getVertexCount());
    const IndexType k = P.getVertexCount();
    const IndexType estimatedEdgesPerPair = P.getEdgeCount() / 2;

    std::vector<std::vector<std::vector<Edge<IndexType>>>> missingEdges(
        static_cast<size_t>(numPerms),
        std::vector<std::vector<Edge<IndexType>>>(
            static_cast<size_t>(numCombs)));

    IndexType permIdx = 0;
    for (const auto& perm : P.permutations()) {
        IndexType combIdx = 0;
        for (const auto& comb : G.combinations(k)) {
            missingEdges[permIdx][combIdx].reserve(estimatedEdgesPerPair);

            for (IndexType i = 0; i < k; ++i) {
                for (IndexType j = 0; j < k; ++j) {
                    const uint8_t pEdges = P.getEdges(perm[i], perm[j]);
                    const uint8_t gEdges = G.getEdges(comb[i], comb[j]);

                    if (pEdges > gEdges) {
                        missingEdges[permIdx][combIdx].emplace_back(
                            comb[i], comb[j], pEdges - gEdges);
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

    for (const auto& combs : CombinationRange<IndexType>(G.combinationsCount(P.getVertexCount()), n)) {
        for (const auto& perms : SequenceRange<IndexType>(P.permutationsCount(), n)) {
            edgeFreqMap.clear();

            IndexType currentSize = 0;

            for (int i = 0; i < n; ++i) {
                for (const auto& edge : allMissingEdges[perms[i]][combs[i]]) {
                    uint64_t& existingCount = edgeFreqMap[edge];
                    if (existingCount == 0) {
                        existingCount = edge.count;
                        currentSize += edge.count;
                    } else if (edge.count > existingCount) {
                        currentSize += (edge.count - existingCount);
                        existingCount = edge.count;
                    }
                }

                if (currentSize >= minSize) {
                    break;
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

template <typename IndexType>
std::vector<Edge<IndexType>> SubgraphAlgorithm<IndexType>::run_approx_v2(int n, Multigraph<IndexType>& P,
                                                                Multigraph<IndexType>& G, HeuristicType heuristic) {
    IndexType k = P.getVertexCount();

    auto currentG = G.getAdjacencyMatrix();
    std::vector<Edge<IndexType>> result;

    int i = 0;
    for (const auto &subset : G.combinations(k)) {
        Multigraph<IndexType> tempG(std::move(currentG));
        auto weightMatrix = Heuristic<IndexType>::createWeightMatrix(P, tempG, subset, heuristic);
        currentG = tempG.getAdjacencyMatrix();

        HungarianAlgorithm hungarian;
        std::vector<int> assignment;
        hungarian.Solve(weightMatrix, assignment);

        for (IndexType u = 0; u < k; ++u) {
            for (IndexType v = 0; v < k; ++v) {
                IndexType pSource = u;
                IndexType pDest = v;
                IndexType gSource = subset[assignment[u]];
                IndexType gDest = subset[assignment[v]];

                uint8_t pEdges = P.getEdges(pSource, pDest);
                uint8_t gEdges = currentG[gSource][gDest];

                if (pEdges > gEdges) {
                    uint8_t missing = pEdges - gEdges;
                    currentG[gSource][gDest] = pEdges;
                    result.emplace_back(gSource, gDest, missing);
                }
            }
        }

        if (++i >= n) {
            break;
        }
    }

    return result;
}

} // namespace Subgraphs
