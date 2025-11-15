#include "graph/multigraph.h"

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

// TODO: maybe some error handling?
void Multigraph::addEdges(int64_t source, int64_t destination, uint8_t count) {
    edgeCount += count;
    adjMatrix[source][destination] += count;
}

// TODO: the same with error
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

NeighborRange Multigraph::neighbors(int64_t v) const {
    return outNeighbors(v);
}

InNeighborRange Multigraph::inNeighbors(int64_t v) const {
    return InNeighborRange(adjMatrix, v);
}

OutNeighborRange Multigraph::outNeighbors(int64_t v) const {
    return OutNeighborRange(adjMatrix[v]);
}

int64_t Multigraph::getVertexCount() const {
    return vertexCount;
}

int64_t Multigraph::getEdgeCount() const {
    return edgeCount;
}

} // namespace Subgraphs
