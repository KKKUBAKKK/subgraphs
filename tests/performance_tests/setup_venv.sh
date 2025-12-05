#!/bin/bash
# Setup script for performance testing environment

echo "Setting up Python virtual environment..."

# Create virtual environment
python3 -m venv venv

# Activate virtual environment
source venv/bin/activate

# Upgrade pip
pip install --upgrade pip

# Install requirements
pip install -r requirements.txt

echo "✓ Virtual environment created and dependencies installed"
echo "To activate the environment, run: source venv/bin/activate"
