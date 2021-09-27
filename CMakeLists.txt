# CMake version
cmake_minimum_required(VERSION 3.18.6 FATAL_ERROR)
cmake_policy(SET CMP0063 NEW) # visibility
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Builds/CMake")

# Declare project
project(RenderFlow)

# Set output directories
set(DEFAULT_CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Set enable output of compile commands during generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find CubbyFlow package in local
find_package(CubbyFlow REQUIRED)
if (NOT CUBBYFLOW_FOUND)
  message(FATAL_ERROR "CubbyFlow is not found.")
endif()

# Includes
include_directories(Includes)
include_directories(Resources)
include_directories(Libraries)
include_directories(${CubbyFlow_INCLUDE_DIR})

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

# Get conan build script
find_file(CONAN_SCRIPT
	NAMES conanbuildinfo.txt
	PATHS ${CMAKE_BINARY_DIR}
		  ${PROJECT_SOURCE_DIR}/build
)

# If find conanbuildinfo.txt, append its directory to CMAKE_MODULE_PATH
if (CONAN_SCRIPT)
	message(STATUS "Find Conan script directory")
	get_filename_component(CONAN_SCRIPT_DIR ${CONAN_SCRIPT} DIRECTORY)
	list(APPEND CMAKE_MODULE_PATH ${CONAN_SCRIPT_DIR})
endif()

# Common dependencies
find_package(cxxopts MODULE REQUIRED)
find_package(glad MODULE REQUIRED)
find_package(Draco MODULE REQUIRED)
find_package(glfw3 MODULE REQUIRED)
find_package(glm MODULE REQUIRED)

# Project modules
add_subdirectory(Sources)
add_subdirectory(Extensions/Fluid2GLTF)
add_subdirectory(Extensions/FlowEditor)