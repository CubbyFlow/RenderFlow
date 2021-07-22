# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindCubbyFlow
-------

Finds the CubbyFlow library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``CubbyFlow::CubbyFlow``
  The CubbyFlow library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``CubbyFlow_FOUND``
  True if the system has the CubbyFlow library.
``CubbyFlow_VERSION``
  The version of the CubbyFlow library which was found.
``CubbyFlow_INCLUDE_DIRS``
  Include directories needed to use CubbyFlow.
``CubbyFlow_LIBRARIES``
  Libraries needed to link to CubbyFlow.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``CubbyFlow_INCLUDE_DIR``
  The directory containing All core header files.
``CubbyFlow_LIBRARY``
  The path to the CubbyFlow library.

#]=======================================================================]

set(CUBBYFLOW_ROOT "${CUBBYFLOW_INSTALL_DIR}" CACHE PATH "CubbyFlow installed directory")
set(CubbyFlow_INCLUDE_DIR "${CUBBYFLOW_ROOT}/include" CACHE PATH "CubbyFlow include directory")

message(STATUS "CUBBYFLOW_ROOT : ${CUBBYFLOW_ROOT}")
message(STATUS "CubbyFlow_INCLUDE_DIR : ${CubbyFlow_INCLUDE_DIR}")

find_library(CubbyFlow_LIBRARY_DEBUG
  NAMES CubbyFlow
  PATHS ${CUBBYFLOW_ROOT}/lib
)
find_library(CubbyFlow_LIBRARY_RELEASE
  NAMES CubbyFlow
  PATHS ${CUBBYFLOW_ROOT}/lib
)

message(STATUS "CubbyFlow_LIBRARY_DEBUG : ${CubbyFlow_LIBRARY_DEBUG}")
message(STATUS "CubbyFlow_LIBRARY_RELEASE : ${CubbyFlow_LIBRARY_RELEASE}")

include(SelectLibraryConfigurations)
select_library_configurations(CubbyFlow)

set(CubbyFlow_VERSION 1.0.0)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CubbyFlow
  FOUND_VAR CubbyFlow_FOUND
  REQUIRED_VARS
  	CubbyFlow_LIBRARY_DEBUG
  	CubbyFlow_LIBRARY_RELEASE
    CubbyFlow_INCLUDE_DIR
  VERSION_VAR CubbyFlow_VERSION
)

if(CubbyFlow_FOUND)
	if (CubbyFlow_LIBRARY_RELEASE)
		set(CubbyFlow_LIBRARIES ${CubbyFlow_LIBRARY_RELEASE})
	endif()
	if (CubbyFlow_LIBRARY_DEBUG)
  		set(CubbyFlow_LIBRARIES ${CubbyFlow_LIBRARY_DEBUG})
  	endif()
  	set(CubbyFlow_INCLUDE_DIRS ${CubbyFlow_INCLUDE_DIR})
endif()

mark_as_advanced(
  CubbyFlow_INCLUDE_DIR
  CubbyFlow_LIBRARY
)