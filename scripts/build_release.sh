#!/bin/bash

# Build script for Release configuration
set -e

echo "🔧 Building Release configuration..."

cd "$(dirname "$0")/.."

mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)

echo "✅ Release build complete!"
echo "Executable: build/bin/release/subgraphs"
