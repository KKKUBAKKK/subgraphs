#include <chrono>
#include <iostream>

#include "algorithms/subgraph_algorithm.h"
#include "graph/multigraph.h"
#include "utils/graph_loader.h"
#include "algorithms/heuristic.h"
#include "utils/graph_printer.h"

using GRAPH_INDEX_TYPE = uint16_t;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_graph_file> [num_subgraphs] [algorithm: exact|approx|approx1] [heuristic: degree|directed|directed_ignore|histogram|structure|greedy]" << std::endl;
        return 1;
    }

    int subgraphsCount = 1;
    if (argc >= 3) {
        try {
            subgraphsCount = std::stoi(argv[2]);
        } catch (...) {
            std::cerr << "Invalid number of subgraphs: " << argv[2] << std::endl;
            return 1;
        }
    }

    std::string algorithm = "exact";
    if (argc >= 4) {
        algorithm = argv[3];
    }

    Subgraphs::HeuristicType heuristic = Subgraphs::HeuristicType::DEGREE_DIFFERENCE;
    if (argc >= 5) {
        std::string heuristicStr = argv[4];
        if (heuristicStr == "degree") {
            heuristic = Subgraphs::HeuristicType::DEGREE_DIFFERENCE;
        } else if (heuristicStr == "directed") {
            heuristic = Subgraphs::HeuristicType::DIRECTED_DEGREE;
        } else if (heuristicStr == "directed_ignore") {
            heuristic = Subgraphs::HeuristicType::DIRECTED_DEGREE_IGNORE_SURPLUS;
        } else if (heuristicStr == "histogram") {
            heuristic = Subgraphs::HeuristicType::NEIGHBOR_HISTOGRAM;
        } else if (heuristicStr == "structure") {
            heuristic = Subgraphs::HeuristicType::STRUCTURE_MATCHING;
        } else if (heuristicStr == "greedy") {
            heuristic = Subgraphs::HeuristicType::GREEDY_NEIGHBOR;
        } else {
            std::cerr << "Unknown heuristic: " << heuristicStr << std::endl;
            std::cerr << "Available heuristics: degree, directed, directed_ignore, histogram, structure, greedy" << std::endl;
            return 1;
        }
    }

    std::string inputGraphFile = argv[1];

    auto start = std::chrono::high_resolution_clock::now();
    try {
        std::cout << "Loading graphs from: " << inputGraphFile << "\n" << std::endl;
        auto [patternGraph, targetGraph] =
            Subgraphs::GraphLoader<GRAPH_INDEX_TYPE>::loadFromFile(inputGraphFile);

        if (targetGraph.combinationsCount(patternGraph.getVertexCount()) < subgraphsCount) {
            std::cerr << "Error: Target graph does not have enough vertices to host "
                      << subgraphsCount << " copies of the pattern graph." << std::endl;
            return 1;
        }

        std::cout << "=== Running Subgraph Algorithm ===" << std::endl;
        std::cout << "Algorithm: " << algorithm << std::endl;
        if (algorithm == "approx") {
            std::cout << "Heuristic: " << static_cast<int>(heuristic) << std::endl;
        }

        std::vector<Subgraphs::Edge<GRAPH_INDEX_TYPE>> result;
        if (algorithm == "exact") {
            result = Subgraphs::SubgraphAlgorithm<GRAPH_INDEX_TYPE>::run(
                subgraphsCount, patternGraph, targetGraph);
        } else if (algorithm == "approx") {
            result = Subgraphs::SubgraphAlgorithm<GRAPH_INDEX_TYPE>::run_approx_v2(
                subgraphsCount, patternGraph, targetGraph, heuristic);
        } else if (algorithm == "approx1") {
            result = Subgraphs::SubgraphAlgorithm<GRAPH_INDEX_TYPE>::run_approx_v1(
                subgraphsCount, patternGraph, targetGraph);
        } else {
            std::cerr << "Unknown algorithm: " << algorithm << std::endl;
            return 1;
        }

        if (result.empty()) {
            std::cout << "No extensions needed." << std::endl;
            return 0;
        }

        Subgraphs::GraphPrinter<GRAPH_INDEX_TYPE>::printResults(patternGraph, targetGraph, result);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "\nExecution time: " << duration.count() << " ms" << std::endl;

    return 0;
}
