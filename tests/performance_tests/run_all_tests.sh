#!/bin/bash
# Master script to run complete performance testing suite

set -e  # Exit on error

echo "============================================================"
echo "COMPLETE PERFORMANCE TESTING SUITE"
echo "============================================================"

# Activate virtual environment
echo ""
echo "▶︎ Activating virtual environment..."
source venv/bin/activate

# Step 1: Generate test matrices
echo ""
echo "============================================================"
echo "STEP 1: Generating Test Matrices"
echo "============================================================"
python3 matrix_gen.py

# Step 2: Run performance tests
echo ""
echo "============================================================"
echo "STEP 2: Running Performance Tests"
echo "============================================================"
echo "⚠️  This may take a while depending on graph sizes..."
python3 run_performance_tests.py

# Step 3: Generate plots
echo ""
echo "============================================================"
echo "STEP 3: Generating Visualizations"
echo "============================================================"
python3 plot_results.py

# Summary
echo ""
echo "============================================================"
echo "✅ COMPLETE!"
echo "============================================================"
echo ""
echo "📁 Results:"
echo "  - CSV data: results/performance_results.csv"
echo "  - Plots: plots/*.png"
echo ""
echo "To view plots:"
echo "  open plots/"
echo ""
