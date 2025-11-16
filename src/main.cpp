#include <chrono>
#include <iostream>

#include "algorithms/subgraph_algorithm.h"
#include "graph/multigraph.h"
#include "utils/graph_loader.h"
#include "utils/graph_printer.h"

// TODO: later add checking if task is possible, if not add vertices to big graph
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_graph_file>" << std::endl;
        return 1;
    }

    int subgraphsCount = 1;
    if (argc >= 3) {
        subgraphsCount = std::stoi(argv[2]);
    }

    std::string inputGraphFile = argv[1];

    auto start = std::chrono::high_resolution_clock::now();
    try {
        std::cout << "Loading graphs from: " << inputGraphFile << "\n" << std::endl;
        auto [patternGraph, targetGraph] = Subgraphs::GraphLoader::loadFromFile(inputGraphFile);

        std::cout << "=== Running Subgraph Algorithm ===" << std::endl;
        auto result = Subgraphs::SubgraphAlgorithm::run(subgraphsCount, patternGraph, targetGraph);

        if (result.empty() || result[0].empty()) {
            std::cout << "No extensions found." << std::endl;
            return 0;
        }

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
