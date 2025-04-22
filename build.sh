#!/bin/bash

# --- Configuration ---
PROJECT_NAME="2d_sfml_game"
PROJECT_ROOT_POSIX="/e/Src/2d_sfml_game" # Use Git Bash path format
BUILD_DIR_POSIX="${PROJECT_ROOT_POSIX}/build"
SFML_VENDOR_DIR_POSIX="${PROJECT_ROOT_POSIX}/vendor/sfml"
# IMPORTANT: Update this to your actual MinGW bin path in Git Bash format
MINGW_BIN_DIR_POSIX="/c/msys64/mingw64/bin"

# Build Configuration
BUILD_TYPE="Debug" # Always use Debug build
NUM_JOBS=$(nproc) # Use all available processor cores for parallel build

# Whitelist of required DLLs
SFML_DEBUG_DLLS=(
    "sfml-system-d-2.dll"
    "sfml-window-d-2.dll"
    "sfml-graphics-d-2.dll"
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
echo "Build Type: ${BUILD_TYPE}"
echo "Parallel Jobs: ${NUM_JOBS}"

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
cmake .. -G "$CMAKE_GENERATOR" -DCMAKE_BUILD_TYPE=${BUILD_TYPE} || error_exit "CMake configuration failed."
cd "$PROJECT_ROOT_POSIX" || error_exit "Failed to change back to project root." # Go back to root for clarity

# --- Build ---
echo "[3/4] Building project with ${MAKE_COMMAND}..."
cd "$BUILD_DIR_POSIX" || error_exit "Failed to change directory to build folder."
${MAKE_COMMAND} -j${NUM_JOBS} || error_exit "Build failed."
cd "$PROJECT_ROOT_POSIX" || error_exit "Failed to change back to project root."

# --- Copy Dependencies (Whitelist) ---
echo "[4/4] Copying required dependencies..."
DLL_DEST_DIR="$BUILD_DIR_POSIX"

# Copy SFML Debug DLLs
if [ -d "${SFML_VENDOR_DIR_POSIX}/bin" ]; then
    echo "Copying SFML Debug DLLs from ${SFML_VENDOR_DIR_POSIX}/bin to ${DLL_DEST_DIR}..."
    for dll in "${SFML_DEBUG_DLLS[@]}"; do
        if [ -f "${SFML_VENDOR_DIR_POSIX}/bin/${dll}" ]; then
            cp "${SFML_VENDOR_DIR_POSIX}/bin/${dll}" "${DLL_DEST_DIR}/" || error_exit "Failed to copy ${dll} from SFML bin."
        else
            echo "Warning: SFML DLL not found: ${SFML_VENDOR_DIR_POSIX}/bin/${dll}"
        fi
    done
else
    echo "Warning: SFML vendor bin directory not found: ${SFML_VENDOR_DIR_POSIX}/bin"
fi

# Copy MinGW DLLs
if [ -d "$MINGW_BIN_DIR_POSIX" ]; then
    echo "Copying MinGW DLLs from ${MINGW_BIN_DIR_POSIX} to ${DLL_DEST_DIR}..."
    for dll in "${MINGW_DLLS[@]}"; do
        if [ -f "${MINGW_BIN_DIR_POSIX}/${dll}" ]; then
            cp "${MINGW_BIN_DIR_POSIX}/${dll}" "${DLL_DEST_DIR}/" || error_exit "Failed to copy ${dll} from MinGW bin."
        else
            echo "Warning: MinGW DLL not found: ${MINGW_BIN_DIR_POSIX}/${dll}"
        fi
    done
else
    echo "Warning: MinGW bin directory not found: ${MINGW_BIN_DIR_POSIX}"
fi

echo "--- Build process complete ---"
echo "Executable should be in: ${DLL_DEST_DIR}"

echo
if [ "$ERROR_OCCURRED" -ne 0 ]; then
    echo "Build script finished with errors."
    echo "Press Enter to exit..."
    read
else
    echo "Build script finished successfully."
fi
