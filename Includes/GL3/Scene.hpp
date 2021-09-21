#ifndef SCENE_HPP
#define SCENE_HPP

#include <Common/GLTFScene.hpp>
#include <Common/Vertex.hpp>
#include <GL3/DebugUtils.hpp>
#include <GL3/GLTypes.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <string>
#include <vector>

namespace GL3
{
class Shader;

/**
 * @brief GLTF Scene rendering class
 */
class Scene : public Common::GLTFScene
{
 public:
    //! Scene node matrix type definition with pair of glm::mat4.
    using NodeMatrix = std::pair<glm::mat4, glm::mat4>;

    /**
     * @brief Construct a new Scene object
     */
    Scene();

    /**
     * @brief Destroy the Scene object
     */
    ~Scene();

    /**
     * @brief Load GLTFScene from the given scene filename and generate buffers
     * @param filename gltf scene file path
     * @param format desired vertex format for parsing scene
     * @return true if gltf scene loading success
     * @return false if gltf scene loading failed
     */
    bool Initialize(const std::string& filename, Common::VertexFormat format);
    
    /**
     * @brief Update the scene for animating
     * @param dt delta time in microseconds
     */
    void Update(double dt);

    /**
     * @brief Render the whole nodes of the parsed gltf-scene
     * @param shader shader for gltf scene nodes rendering(must support pbr pipeline)
     * @param alphaMode alphaMode flag for blending
     */
    void Render(const std::shared_ptr<Shader>& shader, GLenum alphaMode) const;
    
    /**
     * @brief Clean up the generated resources
     */
    void CleanUp();
    
    /**
     * @brief Returns the number of animations
     * @return size_t returns number of animations
     */
    [[nodiscard]] size_t GetNumAnimations() const;
    
    /**
     * @brief Set current scene animation index
     * @param animIndex animation index for playing
     */
    void SetAnimIndex(size_t animIndex);

 private:
    /**
     * @brief Update matrix buffer with modified scene nodes
     */
    void UpdateMatrixBuffer();

    std::vector<GLuint> _textures;
    std::vector<GLuint> _buffers;
    DebugUtils _debug;
    GLuint _vao{ 0 }, _ebo{ 0 };
    GLuint _matrixBuffer{ 0 };
    GLuint _materialBuffer{ 0 };
    double _timeElapsed{ 0.0 };
    size_t _animIndex{ 0 };
};

};  // namespace GL3

#endif  //! end of Scene.hpp