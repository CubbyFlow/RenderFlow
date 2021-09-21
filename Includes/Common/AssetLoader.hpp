#ifndef ASSET_LOADER_HPP
#define ASSET_LOADER_HPP

#include <Common/Vertex.hpp>
#include <GL3/GLTypes.hpp>
#include <string>
#include <vector>

namespace Common
{
/**
 * @brief Collection of asset loader functions.
 *
 */
class AssetLoader
{
 public:
    /**
     * @brief load obj file and returns vertices and indices to given reference
     * argument. Vertices are automatically formatted with given format
     * @param path obj file path to be loaded
     * @param vertices obj vertices passed to
     * @param indices obj indices passed to
     * @param format vertex format to be parsed (ex. Position3Normal3)
     * @return true if obj loading success
     * @return false if obj loading failed
     */
    static bool LoadObjFile(const std::string& path,
                            std::vector<float>& vertices,
                            std::vector<unsigned int>& indices,
                            Common::VertexFormat format);

    /**
     * @brief Load raw binary file and returns them to given reference argument.
     *
     * @param path raw binary file path to be loaded
     * @param data reference binary datum passed to
     * @return true if file loading success
     * @return false if file loading failed
     */
    static bool LoadRawFile(const std::string& path, std::vector<char>& data);

    /**
     * @brief Load float-per-channel image and returns the dynamic allocated
     * pointer and pass dimension to reference argument.
     * It returns nullptr on failed
     * @param path image file path to be loaded
     * @param width pointer, loaded image file width passed to
     * @param height pointer, loaded image file height passed to
     * @param channel pointer, loaded image file num channels passed to
     * @return float* heap-allocated image data array,
     * returns nullptr on failed.
     */
    [[nodiscard]] static float* LoadImageFile(const std::string& path,
                                              int* width, int* height,
                                              int* channel);

    /**
     * @brief Free the heap-allocated image data
     * @param pixels heap-allocated image data array to be freed.
     */
    static void FreeImage(void* pixels);
};

};  // namespace Common

#endif  //! end of AssetLoader.hpp