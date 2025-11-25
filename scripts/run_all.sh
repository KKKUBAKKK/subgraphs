#!/bin/bash

# Script to run all algorithm variants
# Usage: ./run_all.sh <input_graph_file> <num_subgraphs>

set -e

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_graph_file> <num_subgraphs>"
    echo "Example: $0 ../data/graph.txt 3"
    exit 1
fi

INPUT_FILE="$1"
NUM_SUBGRAPHS="$2"

# Check if input file exists
if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: Input file '$INPUT_FILE' not found"
    exit 1
fi

# Determine which executable to use (prefer release, fall back to debug)
if [ -f "build/bin/release/subgraphs" ]; then
    EXECUTABLE="build/bin/release/subgraphs"
    BUILD_TYPE="Release"
elif [ -f "build/bin/debug/subgraphs" ]; then
    EXECUTABLE="build/bin/debug/subgraphs"
    BUILD_TYPE="Debug"
else
    echo "Error: No executable found. Please build the project first."
    echo "Run: ./scripts/build_release.sh or ./scripts/build_debug.sh"
    exit 1
fi

echo "========================================"
echo "Running All Algorithm Variants"
echo "========================================"
echo "Input file: $INPUT_FILE"
echo "Number of subgraphs: $NUM_SUBGRAPHS"
echo "Executable: $EXECUTABLE ($BUILD_TYPE)"
echo "========================================"
echo ""

# Array of all heuristics for approx (v2)
HEURISTICS=("degree" "directed" "directed_ignore" "histogram" "structure" "greedy")

# Run exact algorithm
echo "========================================"
echo "1. Running EXACT algorithm"
echo "========================================"
$EXECUTABLE "$INPUT_FILE" "$NUM_SUBGRAPHS" exact
echo ""
echo ""

# Run approx1 algorithm
echo "========================================"
echo "2. Running APPROX1 (greedy seed) algorithm"
echo "========================================"
$EXECUTABLE "$INPUT_FILE" "$NUM_SUBGRAPHS" approx1
echo ""
echo ""

# Run approx (v2) with all heuristics
counter=3
for heuristic in "${HEURISTICS[@]}"; do
    echo "========================================"
    echo "$counter. Running APPROX (v2) with $heuristic heuristic"
    echo "========================================"
    $EXECUTABLE "$INPUT_FILE" "$NUM_SUBGRAPHS" approx "$heuristic"
    echo ""
    echo ""
    ((counter++))
done

echo "========================================"
echo "All algorithm variants completed!"
echo "========================================"
