#!/bin/bash

# JSSP Solver Test Runner
# This script builds and runs the test suite

set -e

echo "=== JSSP Solver Test Runner ==="
echo

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

# Configure with tests enabled
echo "Configuring build with tests enabled..."
cmake .. -DBUILD_TESTS=ON

# Build the project
echo "Building project and tests..."
make -j$(nproc)

echo
echo "=== Running Tests ==="
echo

# Run tests manually since CTest discovery may not work properly
echo "Running test executable directly..."
./JSSPTests

echo
echo "=== Test Summary ==="
echo "Tests completed successfully!"
echo
echo "To run tests manually, use:"
echo "  ./JSSPTests"
echo "  make check"
echo "  ctest"
