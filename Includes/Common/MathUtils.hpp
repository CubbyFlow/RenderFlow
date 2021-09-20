#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

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
};  // namespace Common

#include <Common/MathUtils-Impl.hpp>
#endif  //! end of MathUtils.hpp