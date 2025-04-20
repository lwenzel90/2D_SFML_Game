#!/bin/bash

# Remove CMake cache and build artifacts
echo "Cleaning CMake cache and build artifacts..."

rm -rf build
rm -f CMakeCache.txt
rm -rf CMakeFiles
rm -f Makefile
rm -f cmake_install.cmake

echo "Cleanup complete."
read -p "Press Enter to exit..."
