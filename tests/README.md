# Multigraph Tests

Comprehensive test suite for the `Multigraph` class and related iterators.

## Running Tests

### Method 1: Using the test script
```bash
./run_tests.sh
```

### Method 2: Using CMake directly
```bash
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
ctest --output-on-failure --verbose
```

### Method 3: Run the test executable directly
```bash
./build/tests/test_multigraph
```

### Method 4: VS Code Tasks
- Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
- Type "Run Test Task"
- Select either:
  - `run-tests` - Run tests only (builds first)
  - `build-and-test` - Full build and test cycle

## Test Coverage

### Constructor Tests
- ✓ `testConstructorEmpty()` - Empty graph with N vertices
- ✓ `testConstructorFromMatrix()` - Graph from adjacency matrix
- ✓ `testCopyConstructor()` - Copy constructor correctness

### Edge Operations
- ✓ `testAddEdges()` - Adding single and multiple edges
- ✓ `testGetEdges()` - Retrieving edge weights
- ✓ `testMultipleEdges()` - Multigraph property (parallel edges)
- ✓ `testSelfLoops()` - Self-loop handling

### Degree Calculations
- ✓ `testDegrees()` - In-degree, out-degree, and total degree

### Neighbor Retrieval
- ✓ `testGetOutNeighbors()` - Vector of out-neighbors
- ✓ `testGetInNeighbors()` - Vector of in-neighbors

### Iterator Tests
- ✓ `testOutNeighborIterator()` - Range-based iteration over out-neighbors
- ✓ `testInNeighborIterator()` - Range-based iteration over in-neighbors
- ✓ `testEmptyIterators()` - Empty neighbor iteration
- ✓ `testIteratorSkipsZeros()` - Iterator skips non-existent edges

### Stress Tests
- ✓ `testLargeGraph()` - Graph with 100 vertices

## Test Output

Successful test run:
```
========================================
Running Multigraph Tests
========================================

Testing: Constructor with vertex count...
✓ Passed

Testing: Constructor from adjacency matrix...
✓ Passed

...

========================================
All tests passed! ✓
========================================
```

## Adding New Tests

To add a new test:

1. Create a test function in `test_multigraph.cpp`:
```cpp
void testMyFeature() {
    std::cout << "Testing: My feature...\n";
    // Your test code with assertions
    assert(condition);
    std::cout << "✓ Passed\n\n";
}
```

2. Call it from `main()`:
```cpp
int main() {
    // ... existing tests ...
    testMyFeature();
    // ...
}
```

3. Rebuild and run tests
