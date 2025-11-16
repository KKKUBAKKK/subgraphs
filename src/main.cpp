#include <chrono>
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

    auto start = std::chrono::high_resolution_clock::now();
    try {
        // Load both graphs from file
        std::cout << "Loading graphs from: " << inputGraphFile << "\n" << std::endl;
        auto [patternGraph, targetGraph] = Subgraphs::GraphLoader::loadFromFile(inputGraphFile);

        // Run subgraph algorithm
        std::cout << "=== Running Subgraph Algorithm ===" << std::endl;
        auto result = Subgraphs::SubgraphAlgorithm::run(1, patternGraph, targetGraph);

        if (result.empty() || result[0].empty()) {
            std::cout << "No extensions found." << std::endl;
            return 0;
        }

        // Print all results
        const auto& extension = result[0][0];
        Subgraphs::GraphPrinter::printResults(patternGraph, targetGraph, extension);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "\nExecution time: " << duration.count() << " ms" << std::endl;

    return 0;
}
