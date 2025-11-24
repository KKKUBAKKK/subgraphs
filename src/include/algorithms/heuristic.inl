
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
            matrix[i][j] = std::abs(static_cast<double>(totalDegreesP[pVertex]) -
                                   static_cast<double>(totalDegreesG[gVertex]));
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
            double inDiff = std::abs(static_cast<double>(inDegreesP[pVertex]) -
                                    static_cast<double>(inDegreesG[gVertex]));
            double outDiff = std::abs(static_cast<double>(outDegreesP[pVertex]) -
                                     static_cast<double>(outDegreesG[gVertex]));
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
            double inDeficit = std::max(0.0, static_cast<double>(inDegreesP[pVertex]) -
                                           static_cast<double>(inDegreesG[gVertex]));
            double outDeficit = std::max(0.0, static_cast<double>(outDegreesP[pVertex]) -
                                            static_cast<double>(outDegreesG[gVertex]));
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
        auto neighborsP = P.getOutNeighbors(i);
        for (const auto& [neighbor, count] : neighborsP) {
            histP[totalDegreesP[neighbor]] += count;
        }
        auto inNeighborsP = P.getInNeighbors(i);
        for (const auto& [neighbor, count] : inNeighborsP) {
            histP[totalDegreesP[neighbor]] += count;
        }

        for (IndexType j = 0; j < k; ++j) {
            IndexType gVertex = subset[j];

            std::vector<IndexType> histG(maxDegree + 1, 0);
            auto neighborsG = G.getOutNeighbors(gVertex);
            for (const auto& [neighbor, count] : neighborsG) {
                histG[totalDegreesG[neighbor]] += count;
            }
            auto inNeighborsG = G.getInNeighbors(gVertex);
            for (const auto& [neighbor, count] : inNeighborsG) {
                histG[totalDegreesG[neighbor]] += count;
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

// Heuristic 5: Structure Matching (Triangle counting)
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_StructureMatching(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto totalDegreesP = P.getDegrees();
    auto totalDegreesG = G.getDegrees();

    std::vector<IndexType> trianglesP(P.getVertexCount(), 0);
    std::vector<IndexType> trianglesG(G.getVertexCount(), 0);

    for (IndexType u = 0; u < P.getVertexCount(); ++u) {
        auto neighborsU = P.getOutNeighbors(u);
        for (const auto& [v, count1] : neighborsU) {
            auto neighborsV = P.getOutNeighbors(v);
            for (const auto& [w, count2] : neighborsV) {
                if (P.getEdges(w, u) > 0) {
                    trianglesP[u]++;
                    trianglesP[v]++;
                    trianglesP[w]++;
                }
            }
        }
    }

    for (IndexType u = 0; u < G.getVertexCount(); ++u) {
        auto neighborsU = G.getOutNeighbors(u);
        for (const auto& [v, count1] : neighborsU) {
            auto neighborsV = G.getOutNeighbors(v);
            for (const auto& [w, count2] : neighborsV) {
                if (G.getEdges(w, u) > 0) {
                    trianglesG[u]++;
                    trianglesG[v]++;
                    trianglesG[w]++;
                }
            }
        }
    }

    double alpha = 0.5;
    double beta = 0.5;

    for (IndexType i = 0; i < k; ++i) {
        for (IndexType j = 0; j < k; ++j) {
            IndexType pVertex = i;
            IndexType gVertex = subset[j];
            double degreeDiff = std::abs(static_cast<double>(totalDegreesP[pVertex]) -
                                        static_cast<double>(totalDegreesG[gVertex]));
            double triangleDeficit = std::max(0.0, static_cast<double>(trianglesP[pVertex]) -
                                                   static_cast<double>(trianglesG[gVertex]));
            matrix[i][j] = alpha * degreeDiff + beta * triangleDeficit;
        }
    }

    return matrix;
}

// Heuristic 6: Greedy Neighbor Matching
template <typename IndexType>
std::vector<std::vector<double>> Heuristic<IndexType>::createWeightMatrix_GreedyNeighbor(
    const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
    const std::vector<IndexType>& subset) {

    IndexType k = P.getVertexCount();
    std::vector<std::vector<double>> matrix(k, std::vector<double>(k, 0.0));

    auto totalDegreesP = P.getDegrees();
    auto totalDegreesG = G.getDegrees();

    for (IndexType i = 0; i < k; ++i) {
        IndexType pVertex = i;

        auto pNeighbors = P.getOutNeighbors(pVertex);
        auto pInNeighbors = P.getInNeighbors(pVertex);

        for (IndexType j = 0; j < k; ++j) {
            IndexType gVertex = subset[j];

            auto gNeighbors = G.getOutNeighbors(gVertex);
            auto gInNeighbors = G.getInNeighbors(gVertex);

            double cost = 0.0;

            for (const auto& [pNeighbor, count] : pNeighbors) {
                double minCost = std::numeric_limits<double>::max();
                for (const auto& [gNeighbor, gCount] : gNeighbors) {
                    double neighborCost = std::abs(static_cast<double>(totalDegreesP[pNeighbor]) -
                                                   static_cast<double>(totalDegreesG[gNeighbor]));
                    minCost = std::min(minCost, neighborCost);
                }
                if (minCost != std::numeric_limits<double>::max()) {
                    cost += minCost;
                }
            }

            for (const auto& [pNeighbor, count] : pInNeighbors) {
                double minCost = std::numeric_limits<double>::max();
                for (const auto& [gNeighbor, gCount] : gInNeighbors) {
                    double neighborCost = std::abs(static_cast<double>(totalDegreesP[pNeighbor]) -
                                                   static_cast<double>(totalDegreesG[gNeighbor]));
                    minCost = std::min(minCost, neighborCost);
                }
                if (minCost != std::numeric_limits<double>::max()) {
                    cost += minCost;
                }
            }

            matrix[i][j] = cost;
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
