#ifndef SKYDOME_HPP
#define SKYDOME_HPP

#include <GL3/DebugUtils.hpp>
#include <GL3/GLTypes.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace GL3
{
class Texture;
class Shader;

/**
 * @brief Skydome environment map for Image Based Lighting
 * @details This skydome require hdr environment map image input and generates
 * multiple textures. [hdrTexture] : texture 2d resource contain given hdr
 * envionment image [accelTexture] : acceleration texture for speed-up generate
 * several filters and brdf LUT [brdflUT] : brdf lookup table texture which can
 * be precalculated [prefilteredCube] : prefiltered glossy texture which can be
 * precalculated
 *
 */
class SkyDome
{
 public:
    /**
     * @brief Construct a new Sky Dome object
     */
    SkyDome() = default;

    /**
     * @brief Destroy the Sky Dome object
     */
    ~SkyDome();

    /**
     * @brief Initialize Skydome with hdr environment map filepath.
     * This method will load hdr image and create each corresponded textures
     * @param envPath environment hdr image file path
     * @return true if skydome creation successful
     * @return false if skydome creation failed
     */
    bool Initialize(const std::string& envPath);

    /**
     * @brief Render skydoem environment to screen
     * @param shader precompiled shader for rendering skybox
     * @param alphaMode alphaMode flag for blending
     */
    void Render(const std::shared_ptr<Shader>& shader, GLenum alphaMode);

    /**
     * @brief Clean up the generated resources
     */
    void CleanUp();

    struct IBLTextureSet
    {
        GLuint brdfLUT{ 0 };
        GLuint irradianceCube{ 0 };
        GLuint prefilteredCube{ 0 };
        GLuint hdrTexture{ 0 };
        GLuint accelTexture{ 0 };
    };

    /**
     * @brief Returns const reference of IBL texture set.
     * @return const IBLTextureSet& const reference of prebaked textures
     * must avoid using this reference after skydome destruction.
     */
    [[nodiscard]] const IBLTextureSet& GetIBLTextureSet() const;

 private:
    /**
     * @brief Create a resources(vao, vbo, ebo) for cube mesh
     */
    void CreateCube();

    /**
     * @brief render prebaked texels to given texture cube map
     * @param fbo preconfigured framebuffer for offline rendering
     * @param texture target texture of baking
     * @param shader precompiled shader for rendering texels
     * @param dim viewport dimension
     * @param numMips number of mips
     */
    void RenderToCube(GLuint fbo, GLuint texture, Shader* shader,
                      unsigned int dim, unsigned int numMips);

    /**
     * @brief Create a Environment Accel Texture object
     * @param pixels raw pointer to hdr image data
     * @param size environment hdr image resolution
     * @param accelTexture target texture for storing computed texels
     */
    static void CreateEnvironmentAccelTexture(const float* pixels, glm::uvec2 size,
                                       GLuint accelTexture);
    
    /**
     * @brief baking BRDF lookup table
     * @param dim desired brdf LUT texture dimension
     */
    void IntegrateBRDF(unsigned int dim);

    /**
     * @brief baking diffuse map texture
     * @param dim desired diffuse map texture dimension
     */
    void PrefilterDiffuse(unsigned int dim);

    /**
     * @brief baking glossy map texture
     * @param dim desired glossy texture dimension
     */
    void PrefilterGlossy(unsigned int dim);

    IBLTextureSet _textureSet;
    GLuint _vao{ 0 }, _vbo{ 0 }, _ebo{ 0 };
    DebugUtils _debug;
};

};  // namespace GL3

#endif  //! end of SkyDome.hpp