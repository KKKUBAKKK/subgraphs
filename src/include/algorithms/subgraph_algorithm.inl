
namespace Subgraphs {

/**
 * Phase 1 of Exact Algorithm: Compute Missing Edges for All Embeddings
 *
 * This function generates ALL possible ways to embed pattern graph P into target graph G,
 * and for each embedding, computes the list of edges that would need to be added.
 *
 * Key Concepts:
 * - An "embedding" is defined by:
 *   1. A k-combination of G vertices (which k vertices to use)
 *   2. A permutation of P vertices (how to order/map P vertices onto those k vertices)
 *
 * - For each embedding, we check every edge in P. If P has more edges between two
 *   vertices than G has between their corresponding mapped vertices, we record the
 *   difference as "missing edges" that would need to be added.
 *
 * Output Structure:
 *   missingEdges[permutation_index][combination_index] = list of edges to add
 *
 * This precomputation allows Phase 2 to efficiently try different combinations
 * of m embeddings without recalculating edge differences.
 *
 * Time Complexity: O(C(n,k) × k! × k²) where n=|V_G|, k=|V_P|
 * Space Complexity: O(C(n,k) × k! × k²) in worst case
 */
template <typename IndexType>
std::vector<std::vector<std::vector<Edge<IndexType>>>>
SubgraphAlgorithm<IndexType>::getAllMissingEdges(Multigraph<IndexType>& P,
                                                 Multigraph<IndexType>& G) {
    const IndexType numPerms = P.permutationsCount();                // k! permutations
    const IndexType numCombs = G.combinationsCount(P.getVertexCount()); // C(n,k) combinations
    const IndexType k = P.getVertexCount();
    const IndexType estimatedEdgesPerPair = P.getEdgeCount() / 2;     // Heuristic for pre-allocation

    // 2D array: missingEdges[permutation][combination] = list of missing edges
    std::vector<std::vector<std::vector<Edge<IndexType>>>> missingEdges(
        static_cast<size_t>(numPerms),
        std::vector<std::vector<Edge<IndexType>>>(
            static_cast<size_t>(numCombs)));

    IndexType permIdx = 0;
    // Iterate through all k! permutations of P vertices
    for (const auto& perm : P.permutations()) {
        IndexType combIdx = 0;
        // Iterate through all C(n,k) combinations of G vertices
        for (const auto& comb : G.combinations(k)) {
            missingEdges[permIdx][combIdx].reserve(estimatedEdgesPerPair);

            // For this specific embedding (perm, comb), check all vertex pairs
            for (IndexType i = 0; i < k; ++i) {
                for (IndexType j = 0; j < k; ++j) {
                    // perm[i] and perm[j] are P vertices in permuted order
                    // comb[i] and comb[j] are their corresponding G vertices
                    const uint8_t pEdges = P.getEdges(perm[i], perm[j]);
                    const uint8_t gEdges = G.getEdges(comb[i], comb[j]);

                    // If P has more edges than G, record the deficit
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

/**
 * Phase 2 of Exact Algorithm: Find Minimal Extension for n Copies
 *
 * Given the precomputed missing edges for all possible embeddings, this function
 * finds the OPTIMAL combination of n embeddings that minimizes the total number
 * of edges that need to be added to G.
 *
 * Key Insight - Edge Sharing:
 *   If embedding A needs edge (u→v) with multiplicity 3, and embedding B needs
 *   the same edge (u→v) with multiplicity 2, we only need to add max(3,2) = 3
 *   copies of that edge to G (not 3+2=5). The embeddings can SHARE edges.
 *
 * Strategy:
 *   1. Try all possible m-combinations of G vertex subsets (where m = n)
 *   2. For each m-combination, try all possible m-sequences of permutations
 *   3. For each configuration, compute the minimum edges needed using max-merge
 *   4. Track the configuration with globally minimum edge count
 *
 * Optimizations:
 *   - Early termination: if current size exceeds best known, skip remaining copies
 *   - Use hash map to efficiently track max multiplicity per edge
 *
 * Time Complexity: O(C(C(n,k), m) × (k!)^m × m × k²)
 * Space Complexity: O(n × |E_P|) for the frequency map
 */
template <typename IndexType>
std::vector<Edge<IndexType>> SubgraphAlgorithm<IndexType>::findMinimalExtension(
    int n, Multigraph<IndexType>& P, Multigraph<IndexType>& G,
    const std::vector<std::vector<std::vector<Edge<IndexType>>>>& allMissingEdges) {
    std::vector<Edge<IndexType>> minimalExtension;         // Best solution found
    IndexType minSize = std::numeric_limits<IndexType>::max(); // Best size found

    // Map from edge (source, dest) to maximum multiplicity needed across all n copies
    std::unordered_map<std::pair<IndexType, IndexType>, IndexType> edgeFreqMap;
    edgeFreqMap.reserve(n * P.getEdgeCount());

    // Try all m-combinations of vertex subsets (ensures n copies use different vertex sets)
    for (const auto& combs : CombinationRange<IndexType>(G.combinationsCount(P.getVertexCount()), n)) {
        // For each combination of subsets, try all m-sequences of permutations
        // (each copy can use a different ordering/mapping)
        for (const auto& perms : SequenceRange<IndexType>(P.permutationsCount(), n)) {
            edgeFreqMap.clear();  // Reset for this configuration

            IndexType currentSize = 0;  // Track total edges needed for this configuration

            // Process each of the n copies
            for (int i = 0; i < n; ++i) {
                // Get missing edges for copy i (using permutation perms[i] and combination combs[i])
                for (const auto& edge : allMissingEdges[perms[i]][combs[i]]) {
                    // Update the maximum multiplicity needed for this edge across all copies
                    IndexType& existingCount = edgeFreqMap[{edge.source, edge.destination}];
                    if (existingCount == 0) {
                        // First copy needs this edge
                        existingCount = edge.count;
                        currentSize += edge.count;
                    } else if (edge.count > existingCount) {
                        // Another copy needs MORE of this edge - increase multiplicity
                        currentSize += (edge.count - existingCount);
                        existingCount = edge.count;
                    }
                    // If edge.count <= existingCount, no change needed (sharing existing edges)
                }

                // Early termination: if we've already exceeded the best known solution, stop
                if (currentSize >= minSize) {
                    break;
                }
            }

            // If this configuration is better than the best known, save it
            if (currentSize < minSize) {
                minSize = currentSize;
                minimalExtension.clear();
                minimalExtension.reserve(edgeFreqMap.size());
                // Convert frequency map to edge list
                for (const auto& [edge, count] : edgeFreqMap) {
                    minimalExtension.emplace_back(edge.first, edge.second, count);
                }
            }
        }
    }

    return minimalExtension;
}

/**
 * Exact Algorithm: Find Minimal Graph Extension for n Copies
 *
 * This is the main entry point for the EXACT algorithm that finds the absolute
 * minimum number of edges needed to add to G so it contains n different copies of P.
 *
 * "Different copies" means each copy uses a different subset of G vertices. Two
 * copies are considered different if they differ in at least one vertex.
 *
 * The algorithm guarantees to find the OPTIMAL solution by exhaustively searching
 * all possible ways to embed n copies of P into G.
 *
 * Algorithm:
 *   1. Precompute missing edges for ALL possible embeddings (getAllMissingEdges)
 *   2. Find the best combination of n embeddings (findMinimalExtension)
 *
 * Returns: List of edges (with multiplicities) that need to be added to G
 *
 * Time Complexity: O(C(n,k) × k! × k²) + O(C(C(n,k),m) × (k!)^m × m × k²)
 *                  where n=|V_G|, k=|V_P|, m=number of copies
 *
 * Practical Limits: Works well for k ≤ 6-7 and n ≤ 20, becomes impractical beyond that
 */
template <typename IndexType>
std::vector<Edge<IndexType>> SubgraphAlgorithm<IndexType>::run(int n, Multigraph<IndexType>& P,
                                                               Multigraph<IndexType>& G) {
    // Phase 1: Compute missing edges for all possible embeddings
    auto allMissingEdges = getAllMissingEdges(P, G);
    // Phase 2: Find optimal combination of n embeddings
    auto result = findMinimalExtension(n, P, G, allMissingEdges);
    return result;
}

/**
 * Approximation Algorithm V2: Hungarian Algorithm with Heuristic Cost Matrix
 *
 * This algorithm uses a polynomial-time heuristic approach to find n copies of P in G.
 * Unlike the exact algorithm, it doesn't guarantee optimality, but runs much faster.
 *
 * Core Idea:
 *   1. Generate the first n k-combinations of G vertices (in lexicographic order)
 *   2. For each combination, solve an assignment problem:
 *      - Create a k×k cost matrix using a HEURISTIC (degree difference, structure, etc.)
 *      - Use Hungarian Algorithm to find optimal P→G vertex mapping (O(k³))
 *      - Add edges to G to support this mapping
 *   3. Share edges between copies using max-merge
 *
 * The quality of the solution depends heavily on the chosen heuristic:
 *   - DEGREE_DIFFERENCE: Simple, fast, based on total degrees
 *   - DIRECTED_DEGREE: Considers in/out degrees separately
 *   - NEIGHBOR_HISTOGRAM: Analyzes neighbor degree distributions
 *   - STRUCTURE_MATCHING: Counts triangles using matrix multiplication
 *   - GREEDY_NEIGHBOR: Enumerates all neighbor permutations (expensive but accurate)
 *
 * Time Complexity: O(n × (N² + k³)) where N=|V_G|, k=|V_P|
 *                  Much faster than exact: O(n × k³) vs O((k!)^n)
 *
 * Space Complexity: O(N²) for adjacency matrix copy
 */
template <typename IndexType>
std::vector<Edge<IndexType>> SubgraphAlgorithm<IndexType>::run_approx_v2(int n, Multigraph<IndexType>& P,
                                                                Multigraph<IndexType>& G, HeuristicType heuristic) {
    IndexType k = P.getVertexCount();

    auto currentG = G.getAdjacencyMatrix();  // Working copy of G's adjacency matrix
    std::unordered_map<std::pair<IndexType, IndexType>, IndexType> result{};  // Accumulated edges to add

    int i = 0;
    // Iterate through the first n k-combinations of G vertices (lexicographic order)
    for (const auto &subset : G.combinations(k)) {
        // Create temporary graph from current state (needed for heuristic computation)
        Multigraph<IndexType> tempG(std::move(currentG));

        // Create k×k cost matrix using the selected heuristic
        // Lower cost = better match between P vertex i and G vertex subset[j]
        auto weightMatrix = Heuristic<IndexType>::createWeightMatrix(P, tempG, subset, heuristic);
        currentG = tempG.getAdjacencyMatrix();

        // Solve assignment problem: find optimal bijection from P vertices to subset vertices
        HungarianAlgorithm hungarian;
        std::vector<int> assignment;  // assignment[i] = which subset vertex P vertex i maps to
        hungarian.Solve(weightMatrix, assignment);

        // Apply the mapping: add edges to G to support this copy of P
        for (IndexType u = 0; u < k; ++u) {
            for (IndexType v = 0; v < k; ++v) {
                // Map P vertices (u, v) to their assigned G vertices
                IndexType pSource = u;
                IndexType pDest = v;
                IndexType gSource = subset[assignment[u]];  // G vertex for P vertex u
                IndexType gDest = subset[assignment[v]];    // G vertex for P vertex v

                uint8_t pEdges = P.getEdges(pSource, pDest);  // Edges in P
                uint8_t gEdges = currentG[gSource][gDest];   // Current edges in G

                // If P has more edges than G, add the difference
                if (pEdges > gEdges) {
                    uint8_t missing = pEdges - gEdges;
                    // Update the working copy of G
                    currentG[gSource][gDest] = pEdges;

                    // Track maximum multiplicity needed across all copies (max-merge for sharing)
                    IndexType& existingCount = result[{gSource, gDest}];
                    if (existingCount == 0) {
                        existingCount = missing;
                    } else if (missing > existingCount) {
                        existingCount = missing;
                    }
                }
            }
        }

        // Stop after processing n combinations
        if (++i >= n) {
            break;
        }
    }

    // Convert the result map to an edge list
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
                bool found = false;
                for (const auto& [p_vertex2, g_vertex2] : selected.mapping) {
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

