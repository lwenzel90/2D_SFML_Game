#!/bin/bash

# --- Configuration ---
PROJECT_NAME="2d_sfml_game"
PROJECT_ROOT_POSIX="/e/Src/2d_sfml_game" # Use Git Bash path format
BUILD_DIR_POSIX="${PROJECT_ROOT_POSIX}/build"
SFML_VENDOR_DIR_POSIX="${PROJECT_ROOT_POSIX}/vendor/sfml"
# IMPORTANT: Update this to your actual MinGW bin path in Git Bash format
MINGW_BIN_DIR_POSIX="/c/msys64/mingw64/bin" 

# List of required DLLs (add/remove as needed)
SFML_DLLS=(
    "sfml-system-2.dll"
    "sfml-window-2.dll"
    "sfml-graphics-2.dll"
    # "sfml-audio-2.dll" # Uncomment if using audio
    # "sfml-network-2.dll" # Uncomment if using network
    # "openal32.dll" # Uncomment if using audio
)
MINGW_DLLS=(
    "libgcc_s_seh-1.dll"
    "libstdc++-6.dll"
    # "libwinpthread-1.dll" # Uncomment if using std::thread
)

CMAKE_GENERATOR="MinGW Makefiles"
MAKE_COMMAND="mingw32-make"
# --- End Configuration ---

# Function for reporting errors and not exiting immediately
error_exit() {
    echo "Error: $1"
    ERROR_OCCURRED=1
}

ERROR_OCCURRED=0

echo "--- Starting Build Script for ${PROJECT_NAME} ---"
echo "Project Root: ${PROJECT_ROOT_POSIX}"

# --- Clean ---
echo "[1/4] Cleaning previous build..."
if [ -d "$BUILD_DIR_POSIX" ]; then
    echo "Removing existing build directory: ${BUILD_DIR_POSIX}"
    rm -rf "$BUILD_DIR_POSIX" || error_exit "Failed to remove build directory."
fi
echo "Creating build directory: ${BUILD_DIR_POSIX}"
mkdir -p "$BUILD_DIR_POSIX" || error_exit "Failed to create build directory."

# --- Configure ---
echo "[2/4] Configuring CMake..."
cd "$BUILD_DIR_POSIX" || error_exit "Failed to change directory to build folder."
cmake .. -G "$CMAKE_GENERATOR" || error_exit "CMake configuration failed."
cd "$PROJECT_ROOT_POSIX" || error_exit "Failed to change back to project root." # Go back to root for clarity

# --- Build ---
echo "[3/4] Building project with ${MAKE_COMMAND}..."
cd "$BUILD_DIR_POSIX" || error_exit "Failed to change directory to build folder."
${MAKE_COMMAND} || error_exit "Build failed."
cd "$PROJECT_ROOT_POSIX" || error_exit "Failed to change back to project root."

# --- Copy ALL DLLs from MinGW bin to build folder ---
echo "[4/4] Copying ALL DLLs from MinGW bin to build folder..."

MINGW_BIN_DIR_POSIX="/c/msys64/mingw64/bin"
DLL_DEST_DIR="${BUILD_DIR_POSIX}"

cp "$MINGW_BIN_DIR_POSIX"/*.dll "$DLL_DEST_DIR"/

# --- Copy ALL DLLs from SFML vendor bin to build folder ---
SFML_BIN_DIR_POSIX="/e/Src/2d_sfml_game/vendor/sfml/bin"
echo "[4/4] Copying ALL DLLs from SFML vendor bin to build folder..."

cp "$SFML_BIN_DIR_POSIX"/*.dll "$DLL_DEST_DIR"/

echo "--- DLL copy complete ---"
echo "Executable and DLLs should be in: ${DLL_DEST_DIR}"

echo
if [ "$ERROR_OCCURRED" -ne 0 ]; then
    echo "Build script finished with errors."
    echo "Press Enter to exit..."
    read
else
    echo "Build script finished successfully."
fi
