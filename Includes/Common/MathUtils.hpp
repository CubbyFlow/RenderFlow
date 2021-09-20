#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

#include <Core/Matrix/Matrix.hpp>
#include <functional>

namespace Common
{
template <typename Type>
using InterpolationMethod = std::function<Type(Type, Type, const float)>;

namespace Interpolation
{
/**
 * @brief Linear interpolation between two values
 *
 * @tparam Type
 * @param prev value in previous step
 * @param next value in next step
 * @param keyframe interpolation keyframe factor
 * @return Type interpolated value
 */
template <typename Type>
Type Lerp(Type prev, Type next, const float keyframe);
/**
 * @brief Quaternion slerp interpolation between two quaternions
 *
 * @tparam Type
 * @param prev value in previous step
 * @param next value in next step
 * @param keyframe interpolation keyframe factor
 * @return Type interpolated value
 */
template <typename Type>
Type SLerp(Type prev, Type next, const float keyframe);
/**
 * @brief Quaternion slerp interpolation between two quaternions
 *
 * @tparam Type
 * @param prev value in previous step
 * @param next value in next step
 * @param keyframe interpolation keyframe factor
 * @return Type interpolated value
 */
template <typename Type>
Type CubicSpline(Type prev, Type next, const float keyframe);
/**
 * @brief Returns prev if keyframe is less or equal than keyframe, next
 * otherwise
 *
 * @tparam Type
 * @param prev value in previous step
 * @param next value in next step
 * @param keyframe interpolation keyframe factor
 * @return Type interpolated value
 */
template <typename Type>
Type Step(Type prev, Type next, const float keyframe);
};  // namespace Interpolation

namespace Transform
{
/**
 * @brief Returns a view transform matrix from given informations
 *
 * @tparam Type need to be float or double type
 * @param origin camera position
 * @param dir camera direction
 * @param up camera up vector
 * @return CubbyFlow::Matrix4x4<Type> view transform matrix
 */
template <typename Type>
CubbyFlow::Matrix4x4<Type> LookAt(CubbyFlow::Vector3<Type> origin,
                                  CubbyFlow::Vector3<Type> dir,
                                  CubbyFlow::Vector3<Type> up);

/**
 * @brief Returns a perspective transform matrix from given informations
 *
 * @tparam Type Type need to be float or double type
 * @param fov fov(field of view) angle in radian
 * @param aspectRatio window aspect ratio (width/height)
 * @param zNear zNear clip distance
 * @param zFar zFar clip distance
 * @return CubbyFlow::Matrix4x4<Type> perspective transform matrix
 */
template <typename Type>
CubbyFlow::Matrix4x4<Type> Perspective(Type fov, Type aspectRatio, Type zNear,
                                       Type zFar);
};  // namespace Transform
};  // namespace Common

#include <Common/MathUtils-Impl.hpp>
#endif  //! end of MathUtils.hpp