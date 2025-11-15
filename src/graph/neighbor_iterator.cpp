#include "graph/neighbor_iterator.h"

namespace Subgraphs {

// NeighborIterator (for out-neighbors)
NeighborIterator::NeighborIterator(const std::vector<uint8_t>& neighbors, int64_t position)
    : neighbors(&neighbors), position(position) {
    skipZeros();
}

void NeighborIterator::skipZeros() {
    while (position < static_cast<int64_t>(neighbors->size()) && (*neighbors)[position] == 0) {
        ++position;
    }
}

NeighborIterator::value_type NeighborIterator::operator*() const {
    return {position, (*neighbors)[position]};
}

NeighborIterator& NeighborIterator::operator++() {
    ++position;
    skipZeros();
    return *this;
}

NeighborIterator NeighborIterator::operator++(int) {
    NeighborIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool NeighborIterator::operator==(const NeighborIterator& other) const {
    return neighbors == other.neighbors && position == other.position;
}

bool NeighborIterator::operator!=(const NeighborIterator& other) const {
    return !(*this == other);
}

// InNeighborIterator (for in-neighbors)
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

InNeighborIterator InNeighborIterator::operator++(int) {
    InNeighborIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool InNeighborIterator::operator==(const InNeighborIterator& other) const {
    return adjMatrix == other.adjMatrix && vertex == other.vertex && position == other.position;
}

bool InNeighborIterator::operator!=(const InNeighborIterator& other) const {
    return !(*this == other);
}

} // namespace Subgraphs
