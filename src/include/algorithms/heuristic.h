#pragma once

#include "../graph/multigraph.h"

namespace Subgraphs {

enum class HeuristicType {
    DEGREE_DIFFERENCE = 1,
    DIRECTED_DEGREE = 2,
    DIRECTED_DEGREE_IGNORE_SURPLUS = 3,
    NEIGHBOR_HISTOGRAM = 4,
    STRUCTURE_MATCHING = 5,
    GREEDY_NEIGHBOR = 6
};

template <typename IndexType>
class Heuristic {
  public:
    static std::vector<std::vector<double>> createWeightMatrix_DegreeDifference(
        const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
        const std::vector<IndexType>& subset);

    static std::vector<std::vector<double>> createWeightMatrix_DirectedDegree(
        const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
        const std::vector<IndexType>& subset);

    static std::vector<std::vector<double>> createWeightMatrix_DirectedDegreeIgnoreSurplus(
        const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
        const std::vector<IndexType>& subset);

    static std::vector<std::vector<double>> createWeightMatrix_NeighborHistogram(
        const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
        const std::vector<IndexType>& subset);

    static std::vector<std::vector<double>> createWeightMatrix_StructureMatching(
        const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
        const std::vector<IndexType>& subset);

    static std::vector<std::vector<double>> createWeightMatrix_GreedyNeighbor(
        const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
        const std::vector<IndexType>& subset);

    static std::vector<std::vector<double>> createWeightMatrix(
        const Multigraph<IndexType>& P, const Multigraph<IndexType>& G,
        const std::vector<IndexType>& subset, HeuristicType heuristic);
};

} // namespace Subgraphs

#include "heuristic.inl"
