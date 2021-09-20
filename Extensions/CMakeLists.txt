# Find essential packages
find_package(CubbyFlow MODULE REQUIRED)
find_package(imgui MODULE REQUIRED)

# Build extensions
add_subdirectory(FlowEditor)
add_subdirectory(Fluid2GLTF)