#!/bin/bash

# Run all tests with CTest
cd "$(dirname "$0")/.."

if [ ! -d "build" ]; then
    echo "❌ Build directory not found. Run build_debug.sh first"
    exit 1
fi

cd build

echo "🧪 Running all tests..."
echo ""

# Run tests, continue even if some fail or timeout
ctest --output-on-failure --force-new-ctest-process || {
    EXIT_CODE=$?
    echo ""
    echo "⚠️  Some tests failed or timed out (exit code: $EXIT_CODE)"
    echo "Check output above for details"
    exit $EXIT_CODE
}

echo ""
echo "✅ All tests passed!"
