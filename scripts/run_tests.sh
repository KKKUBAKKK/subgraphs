#!/bin/bash

# Build and run tests for subgraphs project

set -e

echo "========================================="
echo "Building and Running Tests"
echo "========================================="

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure CMake
echo ""
echo "Configuring CMake..."
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build
echo ""
echo "Building project..."
cmake --build . -j$(sysctl -n hw.ncpu)

# Run tests
echo ""
echo "Running tests..."
ctest --output-on-failure --verbose

echo ""
echo "========================================="
echo "Tests completed!"
echo "========================================="
