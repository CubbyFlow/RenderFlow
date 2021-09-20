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

//!
//! \brief      GLTF Scene rendering class
//!
class Scene : public Common::GLTFScene
{
 public:
    //! Scene node matrix type definition with pair of glm::mat4.
    using NodeMatrix = std::pair<glm::mat4, glm::mat4>;
    //! Default constructor
    Scene();
    //! Default destructor
    ~Scene();
    //! Load GLTFScene from the given scene filename and generate buffers
    bool Initialize(const std::string& filename, Common::VertexFormat format);
    //! Update the scene for animating
    void Update(double dt);
    //! Render the whole nodes of the parsed gltf-scene
    void Render(const std::shared_ptr<Shader>& shader, GLenum alphaMode) const;
    //! Clean up the generated resources
    void CleanUp();
    //! Returns the number of animations
    size_t GetNumAnimations() const;
    //! Set current scene animation index
    void SetAnimIndex(size_t animIndex);

 private:
    //! Update matrix buffer with modified scene nodes
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