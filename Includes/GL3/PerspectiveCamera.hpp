#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include <GL3/Camera.hpp>

namespace GL3
{
/**
 * @brief Camera class providing Perspective-projection.
 * @details Inherit Camera class and overriding update method for perspective
 * projection matrix.
 */
class PerspectiveCamera : public Camera
{
 public:
    /**
     * @brief Construct a new Perspective Camera object
     */
    PerspectiveCamera();

    /**
     * @brief Destroy the Perspective Camera object
     */
    ~PerspectiveCamera() final = default;

    /**
     * @brief Set perspective camera properties
     * @param aspect window screen aspect ratio(width/height)
     * @param fovDegree field of view in degree unit
     * @param zNear near(minimum) z value
     * @param zFar far(maximum) z value
     */
    void SetProperties(float aspect, float fovDegree, float zNear, float zFar);

 private:
    /**
     * @brief Update perpsective projection matrix;
     */
    void OnUpdateMatrix() override;

    float _aspect;
    float _fovDegree;
    float _zNear;
    float _zFar;
};

};  // namespace GL3

#endif  //! end of PerspectiveCamera.hpp