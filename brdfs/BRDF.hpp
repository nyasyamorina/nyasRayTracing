/// @file brdfs/BRDF.hpp
#pragma once

#include "../common/types.hpp"
#include <memory>
#include <tuple>


namespace nyas
{
    using ::std::tuple;

    /// Bidirectional reflectance distribution function interface.
    ///
    /// Note that 'incident' should be pointing out at hitting point. i.e., '-ray.direction'
    class BRDF
    {
    public:
        RGBColor virtual operator()(Vector3D const& normal, Vector3D const& incident, Vector3D const& outgoing) const = 0;

        /// return outgoing ray direction and corresponding BRDF value
        tuple<Vector3D, RGBColor> virtual scatter(Vector3D const& normal, Vector3D const& incident) const = 0;
        // TODO: get scattered ray.direction with Sampler instead of random method
    };

    typedef ::std::shared_ptr<BRDF> BRDFPtr;
    typedef ::std::shared_ptr<BRDF const> BRDFConstptr;

} // namespace nyas
