// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VOLUME_GRID_EMITTER2_HPP
#define CUBBYFLOW_VOLUME_GRID_EMITTER2_HPP

#include <Core/Emitter/GridEmitter2.hpp>
#include <Core/Geometry/ImplicitSurface.hpp>
#include <Core/Grid/ScalarGrid.hpp>
#include <Core/Grid/VectorGrid.hpp>
#include <Core/Matrix/Matrix.hpp>

#include <memory>

namespace CubbyFlow
{
//!
//! \brief 2-D grid-based volumetric emitter.
//!
class VolumeGridEmitter2 final : public GridEmitter2
{
 public:
    class Builder;

    //! Maps to a scalar value for given signed-dist, location, and old value.
    using ScalarMapper = std::function<double(double, const Vector2D&, double)>;

    //! Maps to a vector value for given signed-dist, location, and old value.
    using VectorMapper =
        std::function<Vector2D(double, const Vector2D&, const Vector2D&)>;

    //!
    //! \brief      Constructs an emitter with a source and is-one-shot flag.
    //!
    //! \param[in]  sourceRegion    Emitting region given by the SDF.
    //! \param[in]  isOneShot       True if emitter gets disabled after one
    //!                             shot.
    //!
    explicit VolumeGridEmitter2(ImplicitSurface2Ptr sourceRegion,
                                bool isOneShot = true);

    //! Default copy constructor.
    VolumeGridEmitter2(const VolumeGridEmitter2&) = default;

    //! Default move constructor.
    VolumeGridEmitter2(VolumeGridEmitter2&&) noexcept = default;

    //! Default virtual destructor.
    ~VolumeGridEmitter2() override = default;

    //! Default copy assignment operator.
    VolumeGridEmitter2& operator=(const VolumeGridEmitter2&) = default;

    //! Default move assignment operator.
    VolumeGridEmitter2& operator=(VolumeGridEmitter2&&) noexcept = default;

    //! Adds signed-distance target to the scalar grid.
    void AddSignedDistanceTarget(const ScalarGrid2Ptr& scalarGridTarget);

    //!
    //! \brief      Adds step function target to the scalar grid.
    //!
    //! \param[in]  scalarGridTarget The scalar grid target.
    //! \param[in]  minValue         The minimum value of the step function.
    //! \param[in]  maxValue         The maximum value of the step function.
    //!
    void AddStepFunctionTarget(const ScalarGrid2Ptr& scalarGridTarget,
                               double minValue, double maxValue);

    //!
    //! \brief      Adds a scalar grid target.
    //!
    //! This function adds a custom target to the emitter. The first parameter
    //! defines which grid should it write to. The second parameter,
    //! \p customMapper, defines how to map signed-distance field from the
    //! volume geometry and location of the point to the final scalar value that
    //! is going to be written to the target grid. The third parameter defines
    //! how to blend the old value from the target grid and the new value from
    //! the mapper function.
    //!
    //! \param[in]  scalarGridTarget The scalar grid target
    //! \param[in]  customMapper     The custom mapper.
    //!
    void AddTarget(const ScalarGrid2Ptr& scalarGridTarget,
                   const ScalarMapper& customMapper);

    //!
    //! \brief      Adds a vector grid target.
    //!
    //! This function adds a custom target to the emitter. The first parameter
    //! defines which grid should it write to. The second parameter,
    //! \p customMapper, defines how to map signed-distance field from the
    //! volume geometry and location of the point to the final vector value that
    //! is going to be written to the target grid. The third parameter defines
    //! how to blend the old value from the target grid and the new value from
    //! the mapper function.
    //!
    //! \param[in]  vectorGridTarget The vector grid target
    //! \param[in]  customMapper     The custom mapper.
    //!
    void AddTarget(const VectorGrid2Ptr& vectorGridTarget,
                   const VectorMapper& customMapper);

    //! Returns implicit surface which defines the source region.
    [[nodiscard]] const ImplicitSurface2Ptr& GetSourceRegion() const;

    //! Returns true if this emits only once.
    [[nodiscard]] bool GetIsOneShot() const;

    //! Returns builder fox VolumeGridEmitter2.
    [[nodiscard]] static Builder GetBuilder();

 private:
    using ScalarTarget = std::tuple<ScalarGrid2Ptr, ScalarMapper>;
    using VectorTarget = std::tuple<VectorGrid2Ptr, VectorMapper>;

    void OnUpdate(double currentTimeInSeconds,
                  double timeIntervalInSeconds) override;

    void Emit();

    ImplicitSurface2Ptr m_sourceRegion;
    std::vector<ScalarTarget> m_customScalarTargets;
    std::vector<VectorTarget> m_customVectorTargets;
    bool m_isOneShot = true;
    bool m_hasEmitted = false;
};

//! Shared pointer type for the VolumeGridEmitter2.
using VolumeGridEmitter2Ptr = std::shared_ptr<VolumeGridEmitter2>;

//!
//! \brief Front-end to create VolumeGridEmitter2 objects step by step.
//!
class VolumeGridEmitter2::Builder final
{
 public:
    //! Returns builder with surface defining source region.
    [[nodiscard]] Builder& WithSourceRegion(const Surface2Ptr& sourceRegion);

    //! Returns builder with one-shot flag.
    [[nodiscard]] Builder& WithIsOneShot(bool isOneShot);

    //! Builds VolumeGridEmitter2.
    [[nodiscard]] VolumeGridEmitter2 Build() const;

    //! Builds shared pointer of VolumeGridEmitter2 instance.
    [[nodiscard]] VolumeGridEmitter2Ptr MakeShared() const;

 private:
    ImplicitSurface2Ptr m_sourceRegion;
    bool m_isOneShot = true;
};
}  // namespace CubbyFlow

#endif