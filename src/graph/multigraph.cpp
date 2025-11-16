#include "graph/multigraph.h"

#include <iostream>

namespace Subgraphs {

Multigraph::Multigraph(std::vector<std::vector<uint8_t>>&& adjMatrix)
    : adjMatrix(std::move(adjMatrix)) {
    vertexCount = std::ssize(this->adjMatrix);
    edgeCount = 0;
    for (const auto& row : this->adjMatrix) {
        for (uint8_t weight : row) {
            edgeCount += static_cast<int64_t>(weight);
        }
    }
}

Multigraph::Multigraph(int64_t vertices)
    : vertexCount(vertices), edgeCount(0), adjMatrix(vertices, std::vector<uint8_t>(vertices, 0)) {
}

Multigraph::Multigraph(const Multigraph& other)
    : vertexCount(other.vertexCount), edgeCount(other.edgeCount), adjMatrix(other.adjMatrix) {
}

Multigraph::~Multigraph() = default;

void Multigraph::addEdges(int64_t source, int64_t destination, uint8_t count) {
    edgeCount += count;
    adjMatrix[source][destination] += count;
}

uint8_t Multigraph::getEdges(int64_t source, int64_t destination) const {
    return adjMatrix[source][destination];
}

Degree Multigraph::getDegree(int64_t v) const {
    return {getInDegree(v), getOutDegree(v)};
}

int64_t Multigraph::getInDegree(int64_t v) const {
    int64_t degree = 0;
    for (int64_t i = 0; i < vertexCount; ++i) {
        degree += static_cast<int64_t>(adjMatrix[i][v]);
    }
    return degree;
}

int64_t Multigraph::getOutDegree(int64_t v) const {
    int64_t degree = 0;
    for (uint8_t weight : adjMatrix[v]) {
        degree += static_cast<int64_t>(weight);
    }
    return degree;
}

std::vector<std::pair<int64_t, uint8_t>> Multigraph::getInNeighbors(int64_t v) const {
    std::vector<std::pair<int64_t, uint8_t>> neighbors;
    for (int64_t i = 0; i < vertexCount; ++i) {
        if (adjMatrix[i][v] > 0) {
            neighbors.emplace_back(i, adjMatrix[i][v]);
        }
    }
    return neighbors;
}

std::vector<std::pair<int64_t, uint8_t>> Multigraph::getOutNeighbors(int64_t v) const {
    std::vector<std::pair<int64_t, uint8_t>> neighbors;
    for (int64_t i = 0; i < vertexCount; ++i) {
        if (adjMatrix[v][i] > 0) {
            neighbors.emplace_back(i, adjMatrix[v][i]);
        }
    }
    return neighbors;
}

InNeighborRange Multigraph::inNeighbors(int64_t v) const {
    return InNeighborRange(adjMatrix, v);
}

OutNeighborRange Multigraph::outNeighbors(int64_t v) const {
    return OutNeighborRange(adjMatrix[v]);
}

PermutationRange Multigraph::permutations() const {
    return PermutationRange(vertexCount);
}

CombinationRange Multigraph::combinations(int64_t k) const {
    return CombinationRange(vertexCount, k);
}

int64_t Multigraph::permutationsCount() const {
    int64_t result = 1;
    for (int64_t i = 2; i <= vertexCount; ++i) {
        result *= i;
    }
    return result;
}

int64_t Multigraph::combinationsCount(int64_t k) const {
    if (k > vertexCount || k < 0) {
        return 0;
    }
    if (k == 0 || k == vertexCount) {
        return 1;
    }

    int64_t numerator = 1;
    int64_t denominator = 1;
    for (int64_t i = 0; i < k; ++i) {
        numerator *= (vertexCount - i);
        denominator *= (i + 1);
    }
    return numerator / denominator;
}

int64_t Multigraph::getVertexCount() const {
    return vertexCount;
}

int64_t Multigraph::getEdgeCount() const {
    return edgeCount;
}

bool Multigraph::operator==(const Multigraph& other) const {
    return vertexCount == other.vertexCount && edgeCount == other.edgeCount;
}

bool Multigraph::operator!=(const Multigraph& other) const {
    return !(*this == other);
}

bool Multigraph::operator<(const Multigraph& other) const {
    return vertexCount < other.vertexCount ||
           (vertexCount == other.vertexCount && edgeCount < other.edgeCount);
}


void Multigraph::printAdjacencyMatrix() const {
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

} // namespace Subgraphs
