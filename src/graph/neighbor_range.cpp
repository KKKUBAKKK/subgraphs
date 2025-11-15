#include "graph/neighbor_range.h"

namespace Subgraphs {

OutNeighborRange::OutNeighborRange(const std::vector<uint8_t>& neighbors) : neighbors(&neighbors) {
}

OutNeighborIterator OutNeighborRange::begin() const {
    return OutNeighborIterator(*neighbors, 0);
}

OutNeighborIterator OutNeighborRange::end() const {
    return OutNeighborIterator(*neighbors, static_cast<int64_t>(neighbors->size()));
}

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
