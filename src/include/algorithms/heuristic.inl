
namespace Subgraphs {

// Heuristic 1: Degree Difference
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_DegreeDifference(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto totalDegreesP = P.getDegrees();
    auto totalDegreesG = G.getDegrees();

    for (IndexType i = 0; i < k; ++i) {
        for (IndexType j = 0; j < k; ++j) {
            IndexType pVertex = i;
            IndexType gVertex = subset[j];
            matrix[i][j] = static_cast<double>(std::abs(totalDegreesP[pVertex] - totalDegreesG[gVertex]));
        }
    }

    return matrix;
}

// Heuristic 2: Directed Degree Matching
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_DirectedDegree(
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
            double inDiff = static_cast<double>(std::abs(inDegreesP[pVertex] - inDegreesG[gVertex]));
            double outDiff = static_cast<double>(std::abs(outDegreesP[pVertex] - outDegreesG[gVertex]));
            matrix[i][j] = inDiff + outDiff;
        }
    }

    return matrix;
}

// Heuristic 3: Directed Degree with Surplus Ignore
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
            double inDeficit = std::max(0.0, static_cast<double>(inDegreesP[pVertex] - inDegreesG[gVertex]));
            double outDeficit = std::max(0.0, static_cast<double>(outDegreesP[pVertex] - outDegreesG[gVertex]));
            matrix[i][j] = inDeficit + outDeficit;
        }
    }

    return matrix;
}

// Heuristic 4: Neighbor Degree Histogram
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_NeighborHistogram(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto totalDegreesP = P.getDegrees();
    auto totalDegreesG = G.getDegrees();

    IndexType maxDegree = 0;
    for (const auto& deg : totalDegreesP) {
        maxDegree = std::max(maxDegree, deg);
    }
    for (const auto& deg : totalDegreesG) {
        maxDegree = std::max(maxDegree, deg);
    }

    for (IndexType i = 0; i < k; ++i) {
        std::vector<IndexType> histP(maxDegree + 1, 0);
        auto neighborsP = P.getNeighbors(i);
        for (const auto& [neighbor, count] : neighborsP) {
            histP[count] += 1;
        }

        for (IndexType j = 0; j < k; ++j) {
            IndexType gVertex = subset[j];

            std::vector<IndexType> histG(maxDegree + 1, 0);
            auto neighborsG = G.getNeighbors(gVertex);
            for (const auto& [neighbor, count] : neighborsG) {
                histG[count] += 1;
            }

            double distance = 0.0;
            for (IndexType s = 0; s <= maxDegree; ++s) {
                distance += std::abs(static_cast<double>(histP[s]) - static_cast<double>(histG[s]));
            }
            matrix[i][j] = distance;
        }
    }

    return matrix;
}

template <typename T>
std::vector<std::vector<T>> multiplyAdjacency(
    const std::vector<std::vector<uint8_t>>& A,
    const std::vector<std::vector<uint8_t>>& B) {

    size_t n = A.size();
    std::vector<std::vector<T>> result(n, std::vector<T>(n, 0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                result[i][j] += static_cast<T>(A[i][k]) * static_cast<T>(B[k][j]);
            }
        }
    }

    return result;
}

// Heuristic 5: Structure Matching (Triangle counting using matrix multiplication)
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_StructureMatching(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto totalDegreesP = P.getDegrees();
    auto totalDegreesG = G.getDegrees();

    auto adjP = P.getAdjacencyMatrix();
    auto adjG = G.getAdjacencyMatrix();

    auto adjP2 = multiplyAdjacency<IndexType>(adjP, adjP);
    auto adjG2 = multiplyAdjacency<IndexType>(adjG, adjG);

    std::vector<IndexType> trianglesP(k, 0);
    std::vector<IndexType> trianglesG(G.getVertexCount(), 0);

    for (IndexType i = 0; i < k; ++i) {
        for (IndexType j = 0; j < k; ++j) {
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

    double alpha = 0.5;
    double beta = 0.5;

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

// Heuristic 6: Exhaustive Neighbor Permutation Matching
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_GreedyNeighbor(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto totalDegreesP = P.getDegrees();
    auto totalDegreesG = G.getDegrees();

    auto baseCost = createWeightMatrix_DegreeDifference(P, G, subset);

    for (IndexType i = 0; i < k; ++i) {
        IndexType pVertex = i;
        auto pNeighbors = P.getOutNeighbors(pVertex);

        for (IndexType j = 0; j < k; ++j) {
            IndexType gVertex = subset[j];
            auto gNeighbors = G.getOutNeighbors(gVertex);

            double minCost = std::numeric_limits<double>::max();

            if (!pNeighbors.empty() && !gNeighbors.empty()) {
                size_t pSize = pNeighbors.size();
                size_t gSize = gNeighbors.size();
                std::vector<size_t> gIndices(gSize);
                for (size_t idx = 0; idx < gSize; ++idx) {
                    gIndices[idx] = idx;
                }

                do {
                    double permCost = 0.0;

                    for (size_t pi = 0; pi < pSize; ++pi) {
                        IndexType pNeighborVertex = pNeighbors[pi].first;

                        if (pi < gSize) {
                            IndexType gNeighborVertex = gNeighbors[gIndices[pi]].first;
                            permCost += baseCost[pNeighborVertex][gNeighborVertex];
                        } else {
                            permCost += totalDegreesP[pNeighborVertex];
                        }
                    }

                    if (gSize > pSize) {
                        for (size_t gi = pSize; gi < gSize; ++gi) {
                            IndexType gNeighborVertex = gNeighbors[gIndices[gi]].first;
                            permCost += totalDegreesG[gNeighborVertex];
                        }
                    }

                    minCost = std::min(minCost, permCost);

                } while (std::next_permutation(gIndices.begin(), gIndices.end()));

            } else if (!pNeighbors.empty()) {
                minCost = 0.0;
                for (const auto& [pNeighbor, count] : pNeighbors) {
                    minCost += totalDegreesP[pNeighbor];
                }
            } else if (!gNeighbors.empty()) {
                minCost = 0.0;
                for (const auto& [gNeighbor, count] : gNeighbors) {
                    minCost += totalDegreesG[gNeighbor];
                }
            } else {
                minCost = 0.0;
            }

            matrix[i][j] = minCost;
        }
    }

    return matrix;
}

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
