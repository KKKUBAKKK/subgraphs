#include "graph/neighbor_iterator.h"

namespace Subgraphs {

OutNeighborIterator::OutNeighborIterator(const std::vector<uint8_t>& neighbors, int64_t position)
    : neighbors(&neighbors), position(position) {
    skipZeros();
}

void OutNeighborIterator::skipZeros() {
    while (position < static_cast<int64_t>(neighbors->size()) && (*neighbors)[position] == 0) {
        ++position;
    }
}

OutNeighborIterator::value_type OutNeighborIterator::operator*() const {
    return {position, (*neighbors)[position]};
}

OutNeighborIterator& OutNeighborIterator::operator++() {
    ++position;
    skipZeros();
    return *this;
}

bool OutNeighborIterator::operator==(const OutNeighborIterator& other) const {
    return neighbors == other.neighbors && position == other.position;
}

bool OutNeighborIterator::operator!=(const OutNeighborIterator& other) const {
    return !(*this == other);
}

// InNeighborIterator
InNeighborIterator::InNeighborIterator(const std::vector<std::vector<uint8_t>>& adjMatrix,
                                       int64_t vertex, int64_t position)
    : adjMatrix(&adjMatrix), vertex(vertex), position(position) {
    skipZeros();
}

void InNeighborIterator::skipZeros() {
    while (position < static_cast<int64_t>(adjMatrix->size()) &&
           (*adjMatrix)[position][vertex] == 0) {
        ++position;
    }
}

InNeighborIterator::value_type InNeighborIterator::operator*() const {
    return {position, (*adjMatrix)[position][vertex]};
}

InNeighborIterator& InNeighborIterator::operator++() {
    ++position;
    skipZeros();
    return *this;
}

bool InNeighborIterator::operator==(const InNeighborIterator& other) const {
    return adjMatrix == other.adjMatrix && vertex == other.vertex && position == other.position;
}

bool InNeighborIterator::operator!=(const InNeighborIterator& other) const {
    return !(*this == other);
}

} // namespace Subgraphs
