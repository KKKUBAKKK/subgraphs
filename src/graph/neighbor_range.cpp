#include "graph/neighbor_range.h"

namespace Subgraphs {

// NeighborRange (for out-neighbors)
NeighborRange::NeighborRange(const std::vector<uint8_t>& neighbors) : neighbors(&neighbors) {
}

NeighborIterator NeighborRange::begin() const {
    return NeighborIterator(*neighbors, 0);
}

NeighborIterator NeighborRange::end() const {
    return NeighborIterator(*neighbors, static_cast<int64_t>(neighbors->size()));
}

// InNeighborRange
InNeighborRange::InNeighborRange(const std::vector<std::vector<uint8_t>>& adjMatrix, int64_t vertex)
    : adjMatrix(&adjMatrix), vertex(vertex) {
}

InNeighborIterator InNeighborRange::begin() const {
    return InNeighborIterator(*adjMatrix, vertex, 0);
}

InNeighborIterator InNeighborRange::end() const {
    return InNeighborIterator(*adjMatrix, vertex, static_cast<int64_t>(adjMatrix->size()));
}

} // namespace Subgraphs
