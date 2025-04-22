# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project Name and Language
project(2d_sfml_game  VERSION 1.0 LANGUAGES CXX)

# Set C++ Standard (e.g., C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Prefer standard C++ over compiler extensions

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# --- SFML Setup ---
# Tell CMake where to find the SFML CMake configuration files within our project
# Adjust path if your SFML layout differs slightly
set(SFML_DIR ${CMAKE_SOURCE_DIR}/vendor/sfml/lib/cmake/SFML)

# Find the SFML package and its components
# We need system, window, graphics. Add network and audio later if needed.
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
# Note: Use the actual version you downloaded if newer (e.g., 2.6)

# --- Project Files ---
# Define the executable target and list its source files
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Ball.cpp
    src/Player.cpp
    src/Game.cpp
    src/Attack.cpp
    src/InputHandler.cpp
    src/DebugPanel.cpp
    src/DebugWindow.cpp # Add the new source file here
)

# --- Include Directories ---
# Add our own project's include directory
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    # SFML include directories should be handled automatically by target_link_libraries below
)

# --- Linking ---
# Link SFML libraries to our executable
# The names (sfml-graphics, etc.) are imported targets created by find_package(SFML)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
# Add sfml-audio, sfml-network later when you use those modules

# --- Output Directories (Optional but good practice) ---
# Place the final executable in the root of the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Display a message
message(STATUS "Configured ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "SFML Found: ${SFML_FOUND}")
message(STATUS "SFML Include Dir: ${SFML_INCLUDE_DIR}") # Handled by target_link_libraries
message(STATUS "SFML Library Dir: ${SFML_LIBRARY_DIR}") # Handled by target_link_libraries