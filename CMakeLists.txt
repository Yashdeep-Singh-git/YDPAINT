cmake_minimum_required(VERSION 3.16)

# Project Name
project(YDEDITOR VERSION 1.0 LANGUAGES CXX)

# [cite_start]Assignment Requirement: "Use sophisticated C++ object-oriented features" [cite: 73]
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# 1. SETUP QT (Even if you only use backend for now, keep this ready)
# -----------------------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

set(CMAKE_AUTOMOC ON) # Handle Qt's "Signals and Slots"
set(CMAKE_AUTORCC ON) # Handle Resource files
set(CMAKE_AUTOUIC ON) # Handle UI files (if any)

# -----------------------------------------------------------------------------
# 2. DEFINE SOURCES
# -----------------------------------------------------------------------------
# Tell CMake where your header files are
include_directories(.)

# Automatically find all .cpp files in your src folder and subfolders
file(GLOB_RECURSE SOURCES "src/*.cpp")

# -----------------------------------------------------------------------------
# 3. BUILD THE EXECUTABLE
# -----------------------------------------------------------------------------
# explicit list of headers is safer, but glob works for now if we include it

file(GLOB_RECURSE HEADERS "include/gui/*.h" "include/models/*.h")

add_executable(VectorEditor ${SOURCES} ${HEADERS})

# Link the libraries (Qt) to your executable
target_link_libraries(VectorEditor PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core)