#pragma once

#include <vector>

#include "combination_iterator.h"
#include "neighbor_iterator.h"
#include "neighbor_range.h"
#include "permutation_iterator.h"

namespace Subgraphs {

struct Degree {
    int64_t inDegree{};
    int64_t outDegree{};
};

class Multigraph {
  public:
    Multigraph() = delete;
    explicit Multigraph(std::vector<std::vector<uint8_t>>&& adjMatrix);
    explicit Multigraph(int64_t vertices);
    Multigraph(const Multigraph& other);

    ~Multigraph();

    void addEdges(int64_t source, int64_t destination, uint8_t count = 1);
    uint8_t getEdges(int64_t source, int64_t destination) const;

    Degree getDegree(int64_t v) const;
    int64_t getInDegree(int64_t v) const;
    int64_t getOutDegree(int64_t v) const;

    std::vector<std::pair<int64_t, uint8_t>> getInNeighbors(int64_t v) const;
    std::vector<std::pair<int64_t, uint8_t>> getOutNeighbors(int64_t v) const;

    InNeighborRange inNeighbors(int64_t v) const;
    OutNeighborRange outNeighbors(int64_t v) const;

    PermutationRange permutations() const;
    CombinationRange combinations(int64_t k) const;

    int64_t permutationsCount() const;
    int64_t combinationsCount(int64_t k) const;

    int64_t getVertexCount() const;
    int64_t getEdgeCount() const;

    bool operator==(const Multigraph& other) const;
    bool operator!=(const Multigraph& other) const;
    bool operator<(const Multigraph& other) const;

    void printAdjacencyMatrix() const;


  private:
    int64_t vertexCount{};
    int64_t edgeCount{};
    std::vector<std::vector<uint8_t>> adjMatrix;
};

} // namespace Subgraphs
