#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

#include <Matrix/Matrix.hpp>
#include <functional>

namespace Core
{
template <typename Type>
using InterpolationMethod = std::function<Type(Type, Type, const float)>;

namespace Interpolation
{
template <typename Type>
Type Lerp(Type prev, Type next, const float keyframe);

template <typename Type>
Type SLerp(Type prev, Type next, const float keyframe);

template <typename Type>
Type CubicSpline(Type prev, Type next, const float keyframe);

template <typename Type>
namespace Transform
{
/**
 * @brief
 *
 * @tparam Type
 * @param origin
 * @param direction
 * @return CubbyFlow::Matrix4x4<Type>
 * @param up
 */
template <typename Type>
CubbyFlow::Matrix4x4<Type> LookAt(CubbyFlow::Vector3<Type> origin,
                                  CubbyFlow::Vector3<Type> direction,
                                  CubbyFlow::Vector3<Type> up);

/**
 * @brief
 *
 * @tparam Type
 * @param radian
 * @param aspectRatio
 * @param zNear
}  // namespace Transform
CubbyFlow::Matrix4x4<Type> Perspective(Type radian, Type aspectRatio,
                                       Type zNear, Type zFar);
template <typename Type>
 */
 * @return CubbyFlow::Matrix4x4<Type>
 * @param zFar
Type Step(Type prev, Type next, const float keyframe);
};  // namespace Interpolation
};  // namespace Core

#include <Core/MathUtils-Impl.hpp>
#endif  //! end of MathUtils.hpp