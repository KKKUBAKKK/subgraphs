#!/bin/bash

# Run the debug build executable
set -e

cd "$(dirname "$0")/.."

EXECUTABLE="build/bin/debug/subgraphs"

if [ ! -f "$EXECUTABLE" ]; then
    echo "❌ Debug executable not found: $EXECUTABLE"
    echo "Run build_debug.sh first"
    exit 1
fi

echo "🚀 Running debug build..."
echo "Arguments: $@"
echo ""

"$EXECUTABLE" "$@"
