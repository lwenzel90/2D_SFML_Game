#!/bin/bash
cd "$(dirname "$0")"

# Remove additional file types based on the provided files
find . -type f -name "*.sln" -exec rm -f {} +
find . -type f -name "*.vcxproj" -exec rm -f {} +
find . -type f -name "*.vcxproj.filters" -exec rm -f {} +
# Script to clean Visual Studio build files

echo "Cleaning Visual Studio build files..."

# Remove Visual Studio specific build directories
rm -rf Debug/
rm -rf Release/
rm -rf x64/Debug/
rm -rf x64/Release/

# Remove Visual Studio temporary files
find . -type f -name "*.sdf" -exec rm -f {} +
find . -type f -name "*.opensdf" -exec rm -f {} +
find . -type f -name "*.vcxproj.user" -exec rm -f {} +
find . -type f -name "*.ipch" -exec rm -f {} +
find . -type f -name "*.tlog" -exec rm -f {} +

# Remove Visual Studio generated files
find . -type f -name "*.obj" -exec rm -f {} +
find . -type f -name "*.pch" -exec rm -f {} +
find . -type f -name "*.idb" -exec rm -f {} +
find . -type f -name "*.pdb" -exec rm -f {} +
find . -type f -name "*.log" -exec rm -f {} +

echo "Cleanup complete."