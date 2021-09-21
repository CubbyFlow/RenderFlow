#ifndef POST_PROCESSING_HPP
#define POST_PROCESSING_HPP

#include <GL3/DebugUtils.hpp>
#include <GL3/GLTypes.hpp>
#include <glm/vec2.hpp>
#include <memory>

namespace GL3
{
class Shader;

/**
 * @brief Post-processing wrapper class
 * @details Provides fbo(frame buffer object) binding method for drawing
 * whole scene into attached color texture. With passed scene
 * screen, do tone-mapping, gamma-correction and SSAO by default.
 */
class PostProcessing
{
 public:
    /**
     * @brief Construct a new Post Processing object
     */
    PostProcessing();

    /**
     * @brief Destroy the Post Processing object
     */
    ~PostProcessing();

    /**
     * @brief Initialize the fbo and shader resources
     * @return true if framebuffer and shader init success
     * @return false if framebuffer and shader init failed
     */
    bool Initialize();

    /**
     * @brief Rendering the post-processed screen image
     */
    void Render() const;

    /**
     * @brief Resize the generated resoures
     * @param extent framebuffer and color & depth texture extent
     */
    void Resize(const glm::ivec2& extent);

    /**
     * @brief Cleanup the generated resources
     */
    void CleanUp();

    /**
     * @brief Returns the framebuffer ID
     * @return GLuint opengl resource ID of framebuffer
     */
    [[nodiscard]] inline GLuint GetFramebuffer() const
    {
        return _fbo;
    }

 protected:
 private:
    GLuint _fbo;
    GLuint _color, _depth;
    GLuint _vao;
    DebugUtils _debug;
    std::unique_ptr<GL3::Shader> _shader;
};

};  // namespace GL3

#endif  //! end of PostProcessing.hpp