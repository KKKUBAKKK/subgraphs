#!/bin/bash

# Run the release build executable
set -e

cd "$(dirname "$0")/.."

EXECUTABLE="build/bin/release/subgraphs"

if [ ! -f "$EXECUTABLE" ]; then
    echo "❌ Release executable not found: $EXECUTABLE"
    echo "Run build_release.sh first"
    exit 1
fi

echo "🚀 Running release build..."
echo "Arguments: $@"
echo ""

"$EXECUTABLE" "$@"
