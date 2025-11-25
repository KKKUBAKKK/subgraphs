
namespace Subgraphs {

/**
 * Heuristic 1: Degree Difference (Simplest and Fastest)
 *
 * This is the most basic vertex similarity heuristic. It matches vertices based on
 * how similar their total degrees (in-degree + out-degree) are.
 *
 * Cost Function:
 *   cost(P_vertex_i, G_vertex_j) = |degree(P_i) - degree(G_j)|
 */
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_DegreeDifference(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto totalDegreesP = P.getDegrees();  // Total degree = in-degree + out-degree
    auto totalDegreesG = G.getDegrees();

    // For each P vertex, compute cost of matching it to each candidate G vertex
    for (IndexType i = 0; i < k; ++i) {
        for (IndexType j = 0; j < k; ++j) {
            IndexType pVertex = i;
            IndexType gVertex = subset[j];
            // Cost = absolute difference in total degrees
            matrix[i][j] = static_cast<double>(std::abs(totalDegreesP[pVertex] - totalDegreesG[gVertex]));
        }
    }

    return matrix;
}

/**
 * Heuristic 2: Directed Degree Matching (Direction-Aware)
 *
 * An improvement over Heuristic 1 that considers edge directions separately.
 * It matches vertices based on both in-degree AND out-degree similarity.
 *
 * Cost Function:
 *   cost(P_i, G_j) = |inDegree(P_i) - inDegree(G_j)| + |outDegree(P_i) - outDegree(G_j)|
 */
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_DirectedDegree(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    // Get in-degrees and out-degrees separately
    auto inDegreesP = P.getInDegrees();
    auto outDegreesP = P.getOutDegrees();
    auto inDegreesG = G.getInDegrees();
    auto outDegreesG = G.getOutDegrees();

    for (IndexType i = 0; i < k; ++i) {
        for (IndexType j = 0; j < k; ++j) {
            IndexType pVertex = i;
            IndexType gVertex = subset[j];
            // Sum of absolute differences for both directions
            double inDiff = static_cast<double>(std::abs(inDegreesP[pVertex] - inDegreesG[gVertex]));
            double outDiff = static_cast<double>(std::abs(outDegreesP[pVertex] - outDegreesG[gVertex]));
            matrix[i][j] = inDiff + outDiff;
        }
    }

    return matrix;
}

/**
 * Heuristic 3: Directed Degree with Surplus Ignore (Deficit-Only)
 *
 * Similar to Heuristic 2, but ONLY penalizes degree deficits, not surpluses.
 * If G has MORE edges than P, that's okay (we can just not use them).
 * If G has FEWER edges than P, that's a problem (we'll need to add edges).
 *
 * Cost Function:
 *   cost(P_i, G_j) = max(0, inDegree(P_i) - inDegree(G_j)) +
 *                    max(0, outDegree(P_i) - outDegree(G_j))
 */
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_DirectedDegreeIgnoreSurplus(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto inDegreesP = P.getInDegrees();
    auto outDegreesP = P.getOutDegrees();
    auto inDegreesG = G.getInDegrees();
    auto outDegreesG = G.getOutDegrees();

    for (IndexType i = 0; i < k; ++i) {
        for (IndexType j = 0; j < k; ++j) {
            IndexType pVertex = i;
            IndexType gVertex = subset[j];
            // Only count DEFICITS (when P has more than G), ignore surpluses
            double inDeficit = std::max(0.0, static_cast<double>(inDegreesP[pVertex] - inDegreesG[gVertex]));
            double outDeficit = std::max(0.0, static_cast<double>(outDegreesP[pVertex] - outDegreesG[gVertex]));
            matrix[i][j] = inDeficit + outDeficit;
        }
    }

    return matrix;
}

/**
 * Heuristic 4: Neighbor Edge Count Histogram (1-Hop Structure)
 *
 * This heuristic goes beyond vertex degrees and examines the DISTRIBUTION of
 * edge multiplicities in each vertex's neighborhood.
 *
 * Algorithm:
 *   1. For each vertex, get all its neighbors
 *   2. Create histogram: histogram[c] = number of neighbors with edge count = c
 *   3. Compare histograms using Manhattan distance (L1 norm)
 *
 * Cost Function:
 *   cost(P_i, G_j) = Σ |histogram_P_i[c] - histogram_G_j[c]|
 */
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_NeighborHistogram(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto totalDegreesP = P.getDegrees();
    auto totalDegreesG = G.getDegrees();

    // Find maximum degree across both graphs for histogram sizing
    IndexType maxDegree = 0;
    for (const auto& deg : totalDegreesP) {
        maxDegree = std::max(maxDegree, deg);
    }
    for (const auto& deg : totalDegreesG) {
        maxDegree = std::max(maxDegree, deg);
    }

    // For each P vertex, build its edge count histogram
    for (IndexType i = 0; i < k; ++i) {
        std::vector<IndexType> histP(maxDegree + 1, 0);
        auto neighborsP = P.getNeighbors(i);  // Get all neighbors (in + out)
        // Count neighbors by their edge multiplicity
        for (const auto& [neighbor, count] : neighborsP) {
            histP[count] += 1;  // Increment bucket for this edge count
        }

        // For each candidate G vertex, build its histogram and compute distance
        for (IndexType j = 0; j < k; ++j) {
            IndexType gVertex = subset[j];

            std::vector<IndexType> histG(maxDegree + 1, 0);
            auto neighborsG = G.getNeighbors(gVertex);
            for (const auto& [neighbor, count] : neighborsG) {
                histG[count] += 1;
            }

            // Compute L1 distance between histograms (Manhattan distance)
            double distance = 0.0;
            for (IndexType s = 0; s <= maxDegree; ++s) {
                distance += std::abs(static_cast<double>(histP[s]) - static_cast<double>(histG[s]));
            }
            matrix[i][j] = distance;
        }
    }

    return matrix;
}

/**
 * Helper Function: Matrix Multiplication for Triangle Counting
 *
 * Computes C = A × B for adjacency matrices using standard matrix multiplication.
 * This is used to find 2-hop paths in the graph.
 *
 * Key property: (A²)[i][j] = number of 2-hop paths from vertex i to vertex j
 * If (A²)[i][j] > 0 AND A[j][i] > 0, then vertices i and j participate in triangles
 *
 * Complexity: O(n³) - standard matrix multiplication
 */
template <typename T>
std::vector<std::vector<T>> multiplyAdjacency(
    const std::vector<std::vector<uint8_t>>& A,
    const std::vector<std::vector<uint8_t>>& B) {

    size_t n = A.size();
    std::vector<std::vector<T>> result(n, std::vector<T>(n, 0));

    // Standard matrix multiplication: C[i][j] = Σ A[i][k] * B[k][j]
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                result[i][j] += static_cast<T>(A[i][k]) * static_cast<T>(B[k][j]);
            }
        }
    }

    return result;
}

/**
 * Heuristic 5: Structure Matching via Triangle Counting (2-Hop Patterns)
 *
 * This heuristic analyzes higher-order graph structure by counting triangles.
 * It combines degree information with local clustering patterns.
 *
 * Algorithm:
 *   1. Compute A² (adjacency matrix squared) using matrix multiplication
 *   2. For each vertex i: count triangles as Σ (A²)[i][j] × A[j][i]
 *   3. Combine triangle deficit with degree difference using weighted sum
 *
 * Cost Function:
 *   cost(P_i, G_j) = α × |degree(P_i) - degree(G_j)| +
 *                    β × max(0, triangles(P_i) - triangles(G_j))
 *   where α = β = 0.5 (equal weighting)
 */
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_StructureMatching(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto totalDegreesP = P.getDegrees();
    auto totalDegreesG = G.getDegrees();

    // Get adjacency matrices
    auto adjP = P.getAdjacencyMatrix();
    auto adjG = G.getAdjacencyMatrix();

    // Compute A² using matrix multiplication
    auto adjP2 = multiplyAdjacency<IndexType>(adjP, adjP);
    auto adjG2 = multiplyAdjacency<IndexType>(adjG, adjG);

    // Count triangles for each vertex using: triangles[i] = Σ (A²)[i][j] × A[j][i]
    std::vector<IndexType> trianglesP(k, 0);
    std::vector<IndexType> trianglesG(G.getVertexCount(), 0);

    for (IndexType i = 0; i < k; ++i) {
        for (IndexType j = 0; j < k; ++j) {
            // If there's a 2-hop path i→*→j AND an edge j→i, we have triangle(s)
            if (adjP2[i][j] > 0 && adjP[j][i] > 0) {
                trianglesP[i] += adjP2[i][j];
            }
        }
    }

    for (IndexType i = 0; i < G.getVertexCount(); ++i) {
        for (IndexType j = 0; j < G.getVertexCount(); ++j) {
            if (adjG2[i][j] > 0 && adjG[j][i] > 0) {
                trianglesG[i] += adjG2[i][j];
            }
        }
    }

    // Weighting factors for combining degree and triangle information
    double alpha = 0.5;  // Weight for degree difference
    double beta = 0.5;   // Weight for triangle deficit

    // Build cost matrix combining both metrics
    for (IndexType i = 0; i < k; ++i) {
        for (IndexType j = 0; j < k; ++j) {
            IndexType pVertex = i;
            IndexType gVertex = subset[j];
            double degreeDiff = static_cast<double>(std::abs(totalDegreesP[pVertex] - totalDegreesG[gVertex]));
            double triangleDeficit = std::max(0.0, static_cast<double>(trianglesP[pVertex] - trianglesG[gVertex]));
            matrix[i][j] = alpha * degreeDiff + beta * triangleDeficit;
        }
    }

    return matrix;
}

/**
 * Heuristic 6: Exhaustive Neighbor Permutation Matching (Most Accurate, Slowest)
 *
 * This is the most sophisticated heuristic. For each vertex pair (P_i, G_j),
 * it considers HOW WELL their neighbors can be matched to each other.
 *
 * Algorithm:
 *   1. For each P vertex i and G vertex j:
 *      a. Get out-neighbors of both
 *      b. Create a base cost matrix using Heuristic 1 (degree difference)
 *      c. Try ALL k! permutations of mapping G neighbors to P neighbors
 *      d. For each permutation, compute total mapping cost
 *      e. Use the MINIMUM cost across all permutations
 *   2. Handle size mismatches with penalties
 *
 * Cost Function:
 *   cost(P_i, G_j) = min over all permutations π of:
 *                    Σ baseCost(P_neighbor[k], G_neighbor[π(k)])
 */
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_GreedyNeighbor(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto totalDegreesP = P.getDegrees();
    auto totalDegreesG = G.getDegrees();

    // Use Heuristic 1 as base cost for individual neighbor pairs
    auto baseCost = createWeightMatrix_DegreeDifference(P, G, subset);

    // For each P vertex and G vertex pair
    for (IndexType i = 0; i < k; ++i) {
        IndexType pVertex = i;
        auto pNeighbors = P.getOutNeighbors(pVertex);  // Get P vertex's out-neighbors

        for (IndexType j = 0; j < k; ++j) {
            IndexType gVertex = subset[j];
            auto gNeighbors = G.getOutNeighbors(gVertex);  // Get G vertex's out-neighbors

            double minCost = std::numeric_limits<double>::max();

            // Case 1: Both have neighbors - try all permutations
            if (!pNeighbors.empty() && !gNeighbors.empty()) {
                size_t pSize = pNeighbors.size();
                size_t gSize = gNeighbors.size();

                // Create index array for permutations
                std::vector<size_t> gIndices(gSize);
                for (size_t idx = 0; idx < gSize; ++idx) {
                    gIndices[idx] = idx;
                }

                // Try ALL permutations of G neighbor assignments
                do {
                    double permCost = 0.0;

                    // Map each P neighbor to a G neighbor (or penalty if no match)
                    for (size_t pi = 0; pi < pSize; ++pi) {
                        IndexType pNeighborVertex = pNeighbors[pi].first;

                        if (pi < gSize) {
                            // Map to G neighbor at position gIndices[pi]
                            IndexType gNeighborVertex = gNeighbors[gIndices[pi]].first;
                            permCost += baseCost[pNeighborVertex][gNeighborVertex];
                        } else {
                            // P has more neighbors than G - penalty for unmatched
                            permCost += totalDegreesP[pNeighborVertex];
                        }
                    }

                    // If G has more neighbors than P, add penalty for extras
                    if (gSize > pSize) {
                        for (size_t gi = pSize; gi < gSize; ++gi) {
                            IndexType gNeighborVertex = gNeighbors[gIndices[gi]].first;
                            permCost += totalDegreesG[gNeighborVertex];
                        }
                    }

                    // Track the minimum cost across all permutations
                    minCost = std::min(minCost, permCost);

                } while (std::next_permutation(gIndices.begin(), gIndices.end()));

            // Case 2: P has neighbors but G doesn't - full penalty
            } else if (!pNeighbors.empty()) {
                minCost = 0.0;
                for (const auto& [pNeighbor, count] : pNeighbors) {
                    minCost += totalDegreesP[pNeighbor];
                }
            // Case 3: G has neighbors but P doesn't - penalty for extras
            } else if (!gNeighbors.empty()) {
                minCost = 0.0;
                for (const auto& [gNeighbor, count] : gNeighbors) {
                    minCost += totalDegreesG[gNeighbor];
                }
            // Case 4: Neither has neighbors - perfect match, zero cost
            } else {
                minCost = 0.0;
            }

            matrix[i][j] = minCost;
        }
    }

    return matrix;
}

/**
 * Dispatcher Function: Route to Specific Heuristic
 *
 * This function selects which heuristic to use based on the HeuristicType enum.
 * All heuristics return a k×k cost matrix where matrix[i][j] represents the
 * "cost" or "dissimilarity" of matching P vertex i to G vertex subset[j].
 *
 * Lower cost = better match
 *
 * The Hungarian algorithm will use this matrix to find the minimum-cost
 * perfect matching (bijection) from P vertices to G subset vertices.
 */
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset, HeuristicType heuristic) {

    switch (heuristic) {
        case HeuristicType::DEGREE_DIFFERENCE:
            return createWeightMatrix_DegreeDifference(P, G, subset);
        case HeuristicType::DIRECTED_DEGREE:
            return createWeightMatrix_DirectedDegree(P, G, subset);
        case HeuristicType::DIRECTED_DEGREE_IGNORE_SURPLUS:
            return createWeightMatrix_DirectedDegreeIgnoreSurplus(P, G, subset);
        case HeuristicType::NEIGHBOR_HISTOGRAM:
            return createWeightMatrix_NeighborHistogram(P, G, subset);
        case HeuristicType::STRUCTURE_MATCHING:
            return createWeightMatrix_StructureMatching(P, G, subset);
        case HeuristicType::GREEDY_NEIGHBOR:
            return createWeightMatrix_GreedyNeighbor(P, G, subset);
        default:
            return createWeightMatrix_DegreeDifference(P, G, subset);
    }
}

} // namespace Subgraphs
