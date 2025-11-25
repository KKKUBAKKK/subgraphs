
namespace Subgraphs {

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

    std::unordered_map<std::pair<IndexType, IndexType>, IndexType> edgeFreqMap;
    edgeFreqMap.reserve(n * P.getEdgeCount());

    for (const auto& combs : CombinationRange<IndexType>(G.combinationsCount(P.getVertexCount()), n)) {
        for (const auto& perms : SequenceRange<IndexType>(P.permutationsCount(), n)) {
            edgeFreqMap.clear();

            IndexType currentSize = 0;

            for (int i = 0; i < n; ++i) {
                for (const auto& edge : allMissingEdges[perms[i]][combs[i]]) {
                    IndexType& existingCount = edgeFreqMap[{edge.source, edge.destination}];
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
                    minimalExtension.emplace_back(edge.first, edge.second, count);
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
    std::unordered_map<std::pair<IndexType, IndexType>, IndexType> result{};

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
                    IndexType& existingCount = result[{gSource, gDest}];
                    if (existingCount == 0) {
                        existingCount = missing;
                    } else if (missing > existingCount) {
                        existingCount = missing;
                    }
                }
            }
        }

        if (++i >= n) {
            break;
        }
    }

    std::vector<Edge<IndexType>> edges;
    edges.reserve(result.size());
    for (const auto& [edge, count] : result) {
        edges.emplace_back(edge.first, edge.second, count);
    }
    return edges;
}

/**
 * Approximation Algorithm V1: Greedy Seed-Based Approach
 *
 * This algorithm finds n copies of pattern graph P in target graph G using a greedy
 * seed-based heuristic. The approach:
 * 1. For each possible seed pair (vertex from P, vertex from G), greedily extend
 *    the mapping to cover all vertices of P
 * 2. Compute the cost (number of edges to add) for each complete mapping
 * 3. Select n best non-overlapping mappings
 * 4. Merge the cost matrices using max operation (edges can be shared between copies)
 * 5. Return the edges that need to be added to G
 *
 * Time Complexity: O(|V_P|² × |V_G|² × |V_P|)
 * Space Complexity: O(|V_P| × |V_G| × |V_G|²) for storing all cost matrices
 */
template <typename IndexType>
std::vector<Edge<IndexType>> SubgraphAlgorithm<IndexType>::run_approx_v1(int n, Multigraph<IndexType>& P,
                                                                Multigraph<IndexType>& G) {
    const IndexType k = P.getVertexCount();
    const IndexType numG = G.getVertexCount();

    // SeedConfiguration stores a complete mapping from P vertices to G vertices
    // along with its cost and the required edge additions
    struct SeedConfiguration {
        IndexType totalCost;                              // Total number of edges to add
        std::vector<std::vector<uint8_t>> costMatrix;     // Matrix of edge counts to add at each position
        std::unordered_map<IndexType, IndexType> mapping; // P vertex -> G vertex mapping

        // Sort configurations by total cost (lower is better)
        bool operator<(const SeedConfiguration& other) const {
            return totalCost < other.totalCost;
        }
    };

    // Store all possible seed configurations (one for each seed pair)
    std::vector<SeedConfiguration> allConfigurations;
    allConfigurations.reserve(k * numG);

    // ===== PHASE 1: Generate all seed configurations =====
    // Try every possible seed pair: (u1 from P, u2 from G)
    for (IndexType u1 = 0; u1 < k; ++u1) {
        for (IndexType u2 = 0; u2 < numG; ++u2) {

            // Cost matrix: costMatrix[i][j] = number of edges to add between G vertices i and j
            std::vector<std::vector<uint8_t>> costMatrix(numG, std::vector<uint8_t>(numG, 0));

            // Mapping from P vertices to G vertices for this seed
            std::unordered_map<IndexType, IndexType> mapping;

            // Track which vertices have been mapped
            std::unordered_set<IndexType> mappedP;  // Mapped P vertices
            std::unordered_set<IndexType> mappedG;  // Mapped G vertices

            // Initialize with the seed pair
            mapping[u1] = u2;
            mappedP.insert(u1);
            mappedG.insert(u2);

            // ===== Greedy Extension: Map remaining P vertices to G vertices =====
            // Continue until all P vertices are mapped
            while (mappedP.size() < static_cast<size_t>(k)) {
                IndexType bestV1 = -1;  // Best unmapped P vertex to add next
                IndexType bestV2 = -1;  // Best unmapped G vertex to map it to
                IndexType minCost = std::numeric_limits<IndexType>::max();

                // Try all combinations of unmapped P and G vertices
                for (IndexType v1 = 0; v1 < k; ++v1) {
                    if (mappedP.count(v1)) continue;  // Skip already mapped P vertices

                    for (IndexType v2 = 0; v2 < numG; ++v2) {
                        if (mappedG.count(v2)) continue;  // Skip already mapped G vertices

                        // Compute cost of adding (v1 -> v2) to the current mapping
                        // Cost = sum of missing edges between (v1, v2) and all already-mapped pairs
                        IndexType cost = 0;
                        for (const auto& [mapped1, mapped2] : mapping) {
                            // Check edges in both directions (directed graph)

                            // Forward edges: from already-mapped vertex to the new vertex
                            const uint8_t pEdges1 = P.getEdges(mapped1, v1);
                            const uint8_t gEdges1 = G.getEdges(mapped2, v2);
                            cost += (pEdges1 > gEdges1) ? (pEdges1 - gEdges1) : 0;

                            // Backward edges: from the new vertex to already-mapped vertex
                            const uint8_t pEdges2 = P.getEdges(v1, mapped1);
                            const uint8_t gEdges2 = G.getEdges(v2, mapped2);
                            cost += (pEdges2 > gEdges2) ? (pEdges2 - gEdges2) : 0;
                        }

                        // Keep track of the pair with minimum cost
                        if (cost < minCost) {
                            minCost = cost;
                            bestV1 = v1;
                            bestV2 = v2;
                        }
                    }
                }

                // Add the best pair to the mapping
                if (bestV1 != static_cast<IndexType>(-1)) {
                    mapping[bestV1] = bestV2;
                    mappedP.insert(bestV1);
                    mappedG.insert(bestV2);
                }
            }

            // ===== Compute the complete cost matrix for this mapping =====
            IndexType totalCost = 0;

            // For each pair of P vertices (i, j), check if we need to add edges
            // between their mapped G vertices
            for (IndexType i = 0; i < k; ++i) {
                for (IndexType j = 0; j < k; ++j) {
                    const IndexType gi = mapping[i];  // G vertex mapped from P vertex i
                    const IndexType gj = mapping[j];  // G vertex mapped from P vertex j

                    const uint8_t pEdges = P.getEdges(i, j);  // Edges in P
                    const uint8_t gEdges = G.getEdges(gi, gj); // Edges in G

                    // If P has more edges than G, we need to add the difference
                    if (pEdges > gEdges) {
                        const uint8_t missing = pEdges - gEdges;
                        costMatrix[gi][gj] = missing;
                        totalCost += missing;
                    }
                }
            }

            // Store this complete configuration
            allConfigurations.push_back({totalCost, std::move(costMatrix), std::move(mapping)});
        }
    }

    // ===== PHASE 2: Select n best non-overlapping configurations =====

    // Sort all configurations by total cost (ascending)
    std::sort(allConfigurations.begin(), allConfigurations.end());

    std::vector<SeedConfiguration> selectedConfigs;
    selectedConfigs.reserve(n);

    // Greedily select configurations, ensuring no vertex overlap
    for (const auto& config : allConfigurations) {
        bool usesDifferentSubset = true;

        // Check if this configuration uses the exact same subset of G vertices than any other configuration
        for (const auto& selected : selectedConfigs) {
            usesDifferentSubset = false;
            for (const auto& [p_vertex, g_vertex] : config.mapping) {
                for (const auto& [p_vertex2, g_vertex2] : selected.mapping) {
                    bool found = false;
                    if (g_vertex == g_vertex2) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    usesDifferentSubset = true;
                    break;
                }
            }
            if (!usesDifferentSubset) break;
        }

        // If this configuration uses a different subset of G vertices than any other configuration, add it to the selected set
        if (usesDifferentSubset) {
            selectedConfigs.push_back(config);
            if (static_cast<int>(selectedConfigs.size()) >= n) {
                break;  // Found enough configurations
            }
        }
    }

    // ===== PHASE 3: Merge cost matrices using max operation =====
    // Key insight: Multiple copies can share edges. If copy A needs 3 edges between
    // vertices (i,j) and copy B needs 2 edges between the same vertices, we only
    // need to add max(3,2) = 3 edges total, not 3+2 = 5 edges.
    std::vector<std::vector<uint8_t>> finalMatrix(numG, std::vector<uint8_t>(numG, 0));
    for (const auto& config : selectedConfigs) {
        for (IndexType i = 0; i < numG; ++i) {
            for (IndexType j = 0; j < numG; ++j) {
                // Take the maximum edge count needed across all selected configurations
                finalMatrix[i][j] = std::max(finalMatrix[i][j], config.costMatrix[i][j]);
            }
        }
    }

    // ===== PHASE 4: Convert final matrix to edge list =====
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

