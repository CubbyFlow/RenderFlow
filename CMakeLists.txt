# CMake version
cmake_minimum_required(VERSION 3.18.6 FATAL_ERROR)
cmake_policy(SET CMP0063 NEW) # visibility
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

# Declare project
project(RenderFlow)

# Set output directories
set(DEFAULT_CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Set enable output of compile commands during generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Includes
include_directories(Includes)
include_directories(Resources)
include_directories(Libraries)
include_directories(Libraries/glad)
include_directories(Libraries/cubbyflow/Includes)
include_directories(Libraries/imgui)
include_directories(Libraries/tinygltf)
include_directories(Libraries/glfw/include)
include_directories(Libraries/draco/src)

# Compile options
include(Builds/CMake/CompileOptions.cmake)

# Build type - Release by default
message("CMake build type: " ${CMAKE_BUILD_TYPE})
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DDEBUG)
endif()

option(BUILD_COVERAGE "Build code coverage" OFF)
if (CMAKE_BUILD_TYPE MATCHES Debug AND CMAKE_COMPILER_IS_GNUCXX AND BUILD_COVERAGE)
    include(CodeCoverage)
    setup_target_for_coverage(${PROJECT_NAME}_coverage UnitTests coverage)
endif()

# Some project don't build - SonarCloud only
option(BUILD_SONARCLOUD "Build for SonarCloud" OFF)

# Overrides
set(CMAKE_MACOSX_RPATH ON)

# Set resource directory
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources/")

# Project modules
add_subdirectory(Libraries/glad)
add_subdirectory(Libraries/glfw)
add_subdirectory(Libraries/imgui)
SET(DRACO_JS_GLUE OFF CACHE BOOL "" FORCE)
add_subdirectory(Libraries/draco)
file(COPY "${CMAKE_BINARY_DIR}/draco/draco_features.h" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/draco/src/draco/")
add_subdirectory(Sources)
add_subdirectory(Extensions/Fluid2GLTF)
add_subdirectory(Extensions/FlowEditor)