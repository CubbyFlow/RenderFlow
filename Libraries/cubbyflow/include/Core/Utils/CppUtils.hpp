// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CPP_UTILS_HPP
#define CUBBYFLOW_CPP_UTILS_HPP

#include <algorithm>

namespace CubbyFlow
{
template <class ForwardIter, class T, class Compare = std::less<T>>
ForwardIter BinaryFind(ForwardIter first, ForwardIter last, const T& value,
                       Compare comp = {});
}

#include <Core/Utils/CppUtils-Impl.hpp>

#endif