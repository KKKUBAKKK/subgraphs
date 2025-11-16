#include <iostream>

#include "algorithms/subgraph_algorithm.h"
#include "graph/multigraph.h"
#include "utils/graph_loader.h"
#include "utils/graph_printer.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_graph_file>" << std::endl;
        return 1;
    }

    std::string inputGraphFile = argv[1];

    try {
        // Load both graphs from file
        std::cout << "Loading graphs from: " << inputGraphFile << "\n" << std::endl;
        auto [patternGraph, targetGraph] = Subgraphs::GraphLoader::loadFromFile(inputGraphFile);

        // Print pattern graph (smaller one)
        std::cout << "=== Pattern Graph (P) ===" << std::endl;
        std::cout << "Vertices: " << patternGraph.getVertexCount() << std::endl;
        std::cout << "Edges: " << patternGraph.getEdgeCount() << std::endl;
        std::cout << "Adjacency Matrix:" << std::endl;
        patternGraph.printAdjacencyMatrix();

        // Print target graph (larger one)
        std::cout << "\n=== Target Graph (G) ===" << std::endl;
        std::cout << "Vertices: " << targetGraph.getVertexCount() << std::endl;
        std::cout << "Edges: " << targetGraph.getEdgeCount() << std::endl;
        std::cout << "Adjacency Matrix:" << std::endl;
        targetGraph.printAdjacencyMatrix();

        // Run subgraph algorithm
        std::cout << "\n=== Running Subgraph Algorithm ===" << std::endl;
        auto result = Subgraphs::SubgraphAlgorithm::run(1, patternGraph, targetGraph);

        if (result.empty() || result[0].empty()) {
            std::cout << "No extensions found." << std::endl;
            return 0;
        }

        // Print extension details
        const auto& extension = result[0][0];
        Subgraphs::GraphPrinter::printToTerminal(extension);

        // Create a copy of target graph and apply the extension
        Subgraphs::Multigraph modifiedGraph(targetGraph);
        for (const auto& [source, dest, count] : extension) {
            modifiedGraph.addEdges(source, dest, count);
        }

        // Print modified graph
        std::cout << "\n=== Modified Target Graph (after adding extension) ===" << std::endl;
        std::cout << "Vertices: " << modifiedGraph.getVertexCount() << std::endl;
        std::cout << "Edges: " << modifiedGraph.getEdgeCount() << std::endl;
        std::cout << "Adjacency Matrix:" << std::endl;
        modifiedGraph.printAdjacencyMatrix();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
