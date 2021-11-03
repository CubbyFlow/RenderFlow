# Target name
set(target RenderFlow)
set(ROOT_DIR ${PROJECT_SOURCE_DIR})
set(PUBLIC_HDR_DIR ${PROJECT_SOURCE_DIR}/Includes)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Import essential packages
find_package(tinyobjloader CONFIG REQUIRED)

# Set Common public headers
set(COMMON_PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/Common/AssetLoader.hpp
    ${PUBLIC_HDR_DIR}/Common/GLTFScene-Impl.hpp
    ${PUBLIC_HDR_DIR}/Common/GLTFScene.hpp
    ${PUBLIC_HDR_DIR}/Common/Macros.hpp
    ${PUBLIC_HDR_DIR}/Common/MathUtils-Impl.hpp
    ${PUBLIC_HDR_DIR}/Common/MathUtils.hpp
    ${PUBLIC_HDR_DIR}/Common/Vertex.hpp
)

# Set Common Sources
set(COMMON_SRCS
    ${SRC_DIR}/Common/AssetLoader.cpp
    ${SRC_DIR}/Common/GLTFScene.cpp
    ${SRC_DIR}/Common/Vertex.cpp
)

# Set OpenGL3 public headers
set(GL3_PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/GL3/Application.hpp
    ${PUBLIC_HDR_DIR}/GL3/BoundingBox.hpp
    ${PUBLIC_HDR_DIR}/GL3/Camera.hpp
    ${PUBLIC_HDR_DIR}/GL3/DebugUtils.hpp
    ${PUBLIC_HDR_DIR}/GL3/GLTypes.hpp
    ${PUBLIC_HDR_DIR}/GL3/PerspectiveCamera.hpp
    ${PUBLIC_HDR_DIR}/GL3/PostProcessing.hpp
    ${PUBLIC_HDR_DIR}/GL3/Renderer.hpp
    ${PUBLIC_HDR_DIR}/GL3/Scene.hpp
    ${PUBLIC_HDR_DIR}/GL3/Shader.hpp
    ${PUBLIC_HDR_DIR}/GL3/SkyDome.hpp
    ${PUBLIC_HDR_DIR}/GL3/Window.hpp
)

# Set OpenGL3 Sources
set(GL3_SRCS
    ${SRC_DIR}/GL3/Application.cpp
    ${SRC_DIR}/GL3/BoundingBox.cpp
    ${SRC_DIR}/GL3/Camera.cpp
    ${SRC_DIR}/GL3/DebugUtils.cpp
    ${SRC_DIR}/GL3/PerspectiveCamera.cpp
    ${SRC_DIR}/GL3/PostProcessing.cpp
    ${SRC_DIR}/GL3/Renderer.cpp
    ${SRC_DIR}/GL3/Scene.cpp
    ${SRC_DIR}/GL3/Shader.cpp
    ${SRC_DIR}/GL3/SkyDome.cpp
    ${SRC_DIR}/GL3/Window.cpp
)

# Build library
add_library(${target} ${COMMON_SRCS} ${GL3_SRCS})

# Project options
set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    PUBLIC_HEADER "${COMMON_PUBLIC_HDRS}"
    PUBLIC_HEADER "${GL3_PUBLIC_HDRS}"
)

#Include directories
target_include_directories(${target}
    PUBLIC
    $<BUILD_INTERFACE:${PUBLIC_HDR_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
    ${glm_INCLUDE_DIR}
    ${tinyobjloader_INCLUDE_DIR}
    ${glfw_INCLUDE_DIR}
    PRIVATE
    ${PUBLIC_HDR_DIR}
    ${RESOURCES_DIR}/shaders
)

# Set Compile options for GLM dependency
# MSVC Compile options
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(GLM_COMPILE_OPTIONS
        /wd4201       # -> disable warning: nonstandard extension used: nameless struct/union
        /wd4189       # -> disable warning: nonstandard extension used: nameless struct/union
    )
endif ()
# GCC and Clang compiler options
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(GLM_COMPILE_OPTIONS
        
    )
endif ()

# Compile options
target_compile_options(${target}
    PUBLIC
    ${GLM_COMPILE_OPTIONS}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)

# Link libraries
target_link_libraries(${target}
    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}
	${DEFAULT_LIBRARIES}    
    glfw
    glm::glm
    glad::glad
    cxxopts::cxxopts
)

# Compile definitions
target_compile_definitions(${target}
    PRIVATE
    RESOURCES_DIR="${RESOURCES_DIR}"
    ${DEFAULT_COMPILE_DEFINITIONS}
)

# Install
install(TARGETS  ${target}
        EXPORT   ${target}-config
        RUNTIME  DESTINATION bin
        LIBRARY  DESTINATION lib
        ARCHIVE  DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)
install(EXPORT   ${target}-config
        DESTINATION share/${target}
)