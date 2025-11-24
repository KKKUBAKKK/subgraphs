#pragma once

#include <iostream>
#include <vector>

#include "combination_iterator.h"
#include "permutation_iterator.h"

namespace Subgraphs {

template <typename IndexType = int64_t> struct Degree {
    IndexType inDegree{};
    IndexType outDegree{};
};

template <typename IndexType = int64_t> class Multigraph {
  public:
    Multigraph() = delete;
    explicit Multigraph(std::vector<std::vector<uint8_t>>&& adjMatrix);
    explicit Multigraph(IndexType vertices);
    Multigraph(const Multigraph& other);

    ~Multigraph() = default;

    void addEdges(IndexType source, IndexType destination, uint8_t count = 1);
    uint8_t getEdges(IndexType source, IndexType destination) const;

    Degree<IndexType> getDegree(IndexType v) const;
    IndexType getInDegree(IndexType v) const;
    IndexType getOutDegree(IndexType v) const;

    std::vector<IndexType> getDegrees() const;
    std::vector<IndexType> getInDegrees() const;
    std::vector<IndexType> getOutDegrees() const;

    std::vector<std::pair<IndexType, uint8_t>> getInNeighbors(IndexType v) const;
    std::vector<std::pair<IndexType, uint8_t>> getOutNeighbors(IndexType v) const;

    PermutationRange<IndexType> permutations() const;
    CombinationRange<IndexType> combinations(IndexType k) const;

    uint64_t permutationsCount() const;
    uint64_t combinationsCount(IndexType k) const;

    IndexType getVertexCount() const;
    IndexType getEdgeCount() const;

    bool operator==(const Multigraph& other) const;
    bool operator!=(const Multigraph& other) const;
    bool operator<(const Multigraph& other) const;

    void printAdjacencyMatrix() const;
    std::vector<std::vector<uint8_t>> getAdjacencyMatrix() const;

  private:
    IndexType vertexCount{};
    IndexType edgeCount{};
    std::vector<std::vector<uint8_t>> adjMatrix;
};

} // namespace Subgraphs

#include "multigraph.inl"
