
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

template <typename IndexType>
std::vector<Edge<IndexType>> SubgraphAlgorithm<IndexType>::run_approx_v1(int n, Multigraph<IndexType>& P,
                                                                Multigraph<IndexType>& G) {
    const IndexType k = P.getVertexCount();
    const IndexType numG = G.getVertexCount();

    struct SeedConfiguration {
        IndexType totalCost;
        std::vector<std::vector<uint8_t>> costMatrix;
        std::unordered_map<IndexType, IndexType> mapping;

        bool operator<(const SeedConfiguration& other) const {
            return totalCost < other.totalCost;
        }
    };

    std::vector<SeedConfiguration> allConfigurations;
    allConfigurations.reserve(k * numG);

    for (IndexType u1 = 0; u1 < k; ++u1) {
        for (IndexType u2 = 0; u2 < numG; ++u2) {
            std::vector<std::vector<uint8_t>> costMatrix(numG, std::vector<uint8_t>(numG, 0));
            std::unordered_map<IndexType, IndexType> mapping;
            std::unordered_set<IndexType> mappedP;
            std::unordered_set<IndexType> mappedG;

            mapping[u1] = u2;
            mappedP.insert(u1);
            mappedG.insert(u2);

            while (mappedP.size() < static_cast<size_t>(k)) {
                IndexType bestV1 = -1;
                IndexType bestV2 = -1;
                IndexType minCost = std::numeric_limits<IndexType>::max();

                for (IndexType v1 = 0; v1 < k; ++v1) {
                    if (mappedP.count(v1)) continue;

                    for (IndexType v2 = 0; v2 < numG; ++v2) {
                        if (mappedG.count(v2)) continue;

                        IndexType cost = 0;
                        for (const auto& [mapped1, mapped2] : mapping) {
                            const uint8_t pEdges1 = P.getEdges(mapped1, v1);
                            const uint8_t gEdges1 = G.getEdges(mapped2, v2);
                            const uint8_t pEdges2 = P.getEdges(v1, mapped1);
                            const uint8_t gEdges2 = G.getEdges(v2, mapped2);

                            cost += (pEdges1 > gEdges1) ? (pEdges1 - gEdges1) : 0;
                            cost += (pEdges2 > gEdges2) ? (pEdges2 - gEdges2) : 0;
                        }

                        if (cost < minCost) {
                            minCost = cost;
                            bestV1 = v1;
                            bestV2 = v2;
                        }
                    }
                }

                if (bestV1 != static_cast<IndexType>(-1)) {
                    mapping[bestV1] = bestV2;
                    mappedP.insert(bestV1);
                    mappedG.insert(bestV2);
                }
            }

            IndexType totalCost = 0;
            for (IndexType i = 0; i < k; ++i) {
                for (IndexType j = 0; j < k; ++j) {
                    const IndexType gi = mapping[i];
                    const IndexType gj = mapping[j];
                    const uint8_t pEdges = P.getEdges(i, j);
                    const uint8_t gEdges = G.getEdges(gi, gj);

                    if (pEdges > gEdges) {
                        const uint8_t missing = pEdges - gEdges;
                        costMatrix[gi][gj] = missing;
                        totalCost += missing;
                    }
                }
            }

            allConfigurations.push_back({totalCost, std::move(costMatrix), std::move(mapping)});
        }
    }

    std::sort(allConfigurations.begin(), allConfigurations.end());

    std::vector<SeedConfiguration> selectedConfigs;
    selectedConfigs.reserve(n);

    for (const auto& config : allConfigurations) {
        bool hasOverlap = false;

        for (const auto& selected : selectedConfigs) {
            for (const auto& [p_vertex, g_vertex] : config.mapping) {
                if (selected.mapping.count(p_vertex) && selected.mapping.at(p_vertex) == g_vertex) {
                    hasOverlap = true;
                    break;
                }
            }
            if (hasOverlap) break;
        }

        if (!hasOverlap) {
            selectedConfigs.push_back(config);
            if (static_cast<int>(selectedConfigs.size()) >= n) {
                break;
            }
        }
    }

    std::vector<std::vector<uint8_t>> finalMatrix(numG, std::vector<uint8_t>(numG, 0));
    for (const auto& config : selectedConfigs) {
        for (IndexType i = 0; i < numG; ++i) {
            for (IndexType j = 0; j < numG; ++j) {
                finalMatrix[i][j] = std::max(finalMatrix[i][j], config.costMatrix[i][j]);
            }
        }
    }

    std::vector<Edge<IndexType>> result;
    for (IndexType i = 0; i < numG; ++i) {
        for (IndexType j = 0; j < numG; ++j) {
            if (finalMatrix[i][j] > 0) {
                result.emplace_back(i, j, finalMatrix[i][j]);
            }
        }
    }

    return result;
}

} // namespace Subgraphs

