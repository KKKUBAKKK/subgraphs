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

  private:
    IndexType vertexCount{};
    IndexType edgeCount{};
    std::vector<std::vector<uint8_t>> adjMatrix;
};

// Template implementation

template <typename IndexType>
Multigraph<IndexType>::Multigraph(std::vector<std::vector<uint8_t>>&& adjMatrix)
    : adjMatrix(std::move(adjMatrix)) {
    vertexCount = static_cast<IndexType>(this->adjMatrix.size());
    edgeCount = 0;
    for (const auto& row : this->adjMatrix) {
        for (uint8_t weight : row) {
            edgeCount += static_cast<IndexType>(weight);
        }
    }
}

template <typename IndexType>
Multigraph<IndexType>::Multigraph(IndexType vertices)
    : vertexCount(vertices), edgeCount(0),
      adjMatrix(static_cast<size_t>(vertices),
                std::vector<uint8_t>(static_cast<size_t>(vertices), 0)) {
}

template <typename IndexType>
Multigraph<IndexType>::Multigraph(const Multigraph& other)
    : vertexCount(other.vertexCount), edgeCount(other.edgeCount), adjMatrix(other.adjMatrix) {
}

template <typename IndexType>
void Multigraph<IndexType>::addEdges(IndexType source, IndexType destination, uint8_t count) {
    edgeCount += count;
    adjMatrix[source][destination] += count;
}

template <typename IndexType>
uint8_t Multigraph<IndexType>::getEdges(IndexType source, IndexType destination) const {
    return adjMatrix[source][destination];
}

template <typename IndexType>
Degree<IndexType> Multigraph<IndexType>::getDegree(IndexType v) const {
    return {getInDegree(v), getOutDegree(v)};
}

template <typename IndexType> IndexType Multigraph<IndexType>::getInDegree(IndexType v) const {
    IndexType degree = 0;
    for (IndexType i = 0; i < vertexCount; ++i) {
        degree += static_cast<IndexType>(adjMatrix[i][v]);
    }
    return degree;
}

template <typename IndexType> IndexType Multigraph<IndexType>::getOutDegree(IndexType v) const {
    IndexType degree = 0;
    for (uint8_t weight : adjMatrix[v]) {
        degree += static_cast<IndexType>(weight);
    }
    return degree;
}

template <typename IndexType>
std::vector<std::pair<IndexType, uint8_t>>
Multigraph<IndexType>::getInNeighbors(IndexType v) const {
    std::vector<std::pair<IndexType, uint8_t>> neighbors;
    for (IndexType i = 0; i < vertexCount; ++i) {
        if (adjMatrix[i][v] > 0) {
            neighbors.emplace_back(i, adjMatrix[i][v]);
        }
    }
    return neighbors;
}

template <typename IndexType>
std::vector<std::pair<IndexType, uint8_t>>
Multigraph<IndexType>::getOutNeighbors(IndexType v) const {
    std::vector<std::pair<IndexType, uint8_t>> neighbors;
    for (IndexType i = 0; i < vertexCount; ++i) {
        if (adjMatrix[v][i] > 0) {
            neighbors.emplace_back(i, adjMatrix[v][i]);
        }
    }
    return neighbors;
}

template <typename IndexType>
PermutationRange<IndexType> Multigraph<IndexType>::permutations() const {
    return PermutationRange<IndexType>(vertexCount);
}

template <typename IndexType>
CombinationRange<IndexType> Multigraph<IndexType>::combinations(IndexType k) const {
    return CombinationRange<IndexType>(vertexCount, k);
}

template <typename IndexType> uint64_t Multigraph<IndexType>::permutationsCount() const {
    uint64_t result = 1;
    for (uint64_t i = 2; i <= vertexCount; ++i) {
        result *= i;
    }
    return result;
}

template <typename IndexType> uint64_t Multigraph<IndexType>::combinationsCount(IndexType k) const {
    if (k > vertexCount || k < 0) {
        return 0;
    }
    if (k == 0 || k == vertexCount) {
        return 1;
    }

    uint64_t numerator = 1;
    uint64_t denominator = 1;
    for (IndexType i = 0; i < k; ++i) {
        numerator *= static_cast<uint64_t>(vertexCount - i);
        denominator *= static_cast<uint64_t>(i + 1);
    }
    return numerator / denominator;
}

template <typename IndexType> IndexType Multigraph<IndexType>::getVertexCount() const {
    return vertexCount;
}

template <typename IndexType> IndexType Multigraph<IndexType>::getEdgeCount() const {
    return edgeCount;
}

template <typename IndexType>
bool Multigraph<IndexType>::operator==(const Multigraph& other) const {
    return vertexCount == other.vertexCount && edgeCount == other.edgeCount;
}

template <typename IndexType>
bool Multigraph<IndexType>::operator!=(const Multigraph& other) const {
    return !(*this == other);
}

template <typename IndexType> bool Multigraph<IndexType>::operator<(const Multigraph& other) const {
    return vertexCount < other.vertexCount ||
           (vertexCount == other.vertexCount && edgeCount < other.edgeCount);
}

template <typename IndexType> void Multigraph<IndexType>::printAdjacencyMatrix() const {
    int n = static_cast<int>(adjMatrix.size());
    std::cout << n << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << static_cast<int>(adjMatrix[i][j]);
            if (j + 1 < n)
                std::cout << " ";
        }
        std::cout << "\n";
    }
}

// Type alias for backward compatibility
using MultigraphInt64 = Multigraph<int64_t>;

} // namespace Subgraphs
