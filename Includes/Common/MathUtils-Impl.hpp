//! Interpolation methods referenced on
//! https://github.com/KhronosGroup/glTF-Tutorials/blob/master/gltfTutorial/gltfTutorial_007_Animations.md

#ifndef MATHUTILS_IMPL_HPP
#define MATHUTILS_IMPL_HPP

#include <Common/Macros.hpp>
#include <Core/Utils/Constants.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Common
{
namespace Interpolation
{
template <typename Type>
Type Lerp(Type prev, Type next, const float keyframe)
{
    return (1.0f - keyframe) * prev + keyframe * next;
}

template <typename Type>
Type SLerp(Type prev, Type next, const float keyframe)
{
    float dotProduct = glm::dot(prev, next);

    //! Make sure we take the shortest path in case dot product is negative
    if (dotProduct < 0.0)
    {
        next = -next;
        dotProduct = -dotProduct;
    }

    //! If the two quaternions are too close to each other, just linear
    //! interpolate between the 4D vector
    if (dotProduct > 0.9995)
        return glm::normalize((1.0f - keyframe) * prev + keyframe * next);

    //! Perform the spherical linear interpolation
    float theta0 = std::acos(dotProduct);
    float theta = keyframe * theta0;
    float sinTheta = std::sin(theta);
    float sinTheta0Inv = 1.0 / (std::sin(theta0) + 1e-6);

    float scalePrevQuat =
        std::cos(theta) - dotProduct * sinTheta * sinTheta0Inv;
    float scaleNextQuat = sinTheta * sinTheta0Inv;
    return scalePrevQuat * prev + scaleNextQuat * next;
}

template <typename Type>
Type CubicSpline(Type prev, Type next, const float keyframe)
{
    UNUSED_VARIABLE(prev);
    UNUSED_VARIABLE(next);
    UNUSED_VARIABLE(keyframe);
    static_assert("Not implemented yet");
}

template <typename Type>
Type Step(Type prev, Type next, const float keyframe)
{
    return keyframe >= 1.0 ? next : prev;
}

}  // namespace Interpolation

namespace Transform
{
template <typename Type>
CubbyFlow::Matrix4x4<Type> LookAt(CubbyFlow::Vector3<Type> origin,
                                  CubbyFlow::Vector3<Type> dir,
                                  CubbyFlow::Vector3<Type> up)
{
    const CubbyFlow::Vector3<Type> right = (up.Cross(dir)).Normalized();
    const CubbyFlow::Vector3<Type> cameraUp = (dir.Cross(right)).Normalized();

    CubbyFlow::Matrix4x4<Type> view = { {right.x,      right.y,    right.z,    0.0},
                                        {cameraUp.x,   cameraUp.y, cameraUp.z, 0.0},
                                        {dir.x,        dir.y,      dir.z,      0.0},
                                        {0.0,          0.0,        0.0,        1.0} };

    CubbyFlow::Matrix4x4<Type> translation = { {1.0, 0.0, 0.0, -origin.x},
                                               {0.0, 1.0, 0.0, -origin.y},
                                               {0.0, 0.0, 1.0, -origin.z},
                                               {0.0, 0.0, 0.0,    1.0   } };

    return view * translation;
}

template <typename Type>
CubbyFlow::Matrix4x4<Type> Perspective(Type fov, Type aspectRatio,
                                       Type zNear, Type zFar)
{
    CubbyFlow::Matrix4x4<Type> projection(0.0);
    projection(0, 0) = 1.0 / (std::tan(CubbyFlow::DegreesToRadians(fov) * 0.5) * aspectRatio);
    projection(1, 1) = 1.0 / (std::tan(CubbyFlow::DegreesToRadians(fov) * 0.5));
    projection(2, 2) = (zFar + zNear) / (zNear - zFar);
    projection(2, 3) = (2 * zFar * zNear) / (zNear - zFar);
    projection(3, 2) = -static_cast<Type>(1.0);

    return projection;
}
}  // namespace Transform

};  // namespace Common

#endif  //! end of MathUtils-Impl.hpp