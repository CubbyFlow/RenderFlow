#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL3/DebugUtils.hpp>
#include <GL3/GLTypes.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <unordered_map>

namespace GL3
{
/**
 * @brief FPS view Camera class
 * @details This class provides view matrix and projection matrix getter for
 * retrieving vertices which is transformed into camera space.
 * Also provide UBO(UniformBufferObject) for binding camera to multiple shader.
 */
class Camera
{
 public:
    /**
     * @brief Construct a new Camera object
     */
    Camera();

    /**
     * @brief Destroy the Camera object
     */
    virtual ~Camera();

    /**
     * @brief Fill the uniform buffer object with camera properties
     * @return true if ubo resource initialization success
     * @return false if ubo resource initialization failed
     */
    bool SetupUniformBuffer();

    /**
     * @brief Setup camera position, direction and up vector.
     * @param pos camera origin in the global coordinate
     * @param dir normalized camera direction
     * @param up camera up vector
     */
    void SetupCamera(const glm::vec3& pos, const glm::vec3& dir,
                     const glm::vec3& up);

    /**
     * @brief Returns view matrix
     * @return glm::mat4 camera view matrix
     */
    [[nodiscard]] glm::mat4 GetViewMatrix();

    /**
     * @brief Returns projection matrix
     * @return glm::mat4 camera proejction matrix
     */
    [[nodiscard]] glm::mat4 GetProjectionMatrix();

    /**
     * @brief Bind the uniform buffer to the current context.
     * @param bindingPoint UBO binding point for camera in current bound shader
     */
    void BindCamera(GLuint bindingPoint) const;

    /**
     * @brief Unbind camera
     */
    static void UnbindCamera();

    /**
     * @brief Returns the uniform buffer ID
     * @return GLuint uniform buffer object resource ID
     */
    [[nodiscard]] GLuint GetUniformBuffer() const;

    /**
     * @brief Update the matrix with specific methods, such as perspective or
     * orthogonal
     */
    void UpdateMatrix();

    /**
     * @brief Process the input key
     * @param key input key code defined in glfw3
     */
    void ProcessInput(unsigned int key);

    /**
     * @brief Process the mouse cursor positions
     * @param xpos cursor position x in the screen viewport
     * @param ypos cursor position y in the screen viewport
     */
    void ProcessCursorPos(double xpos, double ypos);

    /**
     * @brief Cleanup the resources
     */
    void CleanUp();

 protected:
    virtual void OnUpdateMatrix(){};
    glm::mat4 _projection, _view;
    glm::vec3 _position, _direction, _up;

 private:
    std::unordered_map<std::string, GLint> _uniformCache;
    DebugUtils _debug;
    float _speed;
    GLuint _uniformBuffer;
};

};  // namespace GL3

#endif  //! end of Camera.hpp