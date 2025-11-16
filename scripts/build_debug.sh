#!/bin/bash

# Build script for Debug configuration
set -e

echo "🔧 Building Debug configuration..."

cd "$(dirname "$0")/.."

mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -j$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)

echo "✅ Debug build complete!"
echo "Executable: build/bin/debug/subgraphs"
