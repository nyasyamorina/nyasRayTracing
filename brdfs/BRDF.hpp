/// @file brdfs/BRDF.hpp
#pragma once

#include "../common/types.hpp"
#include "../common/randoms.hpp"
#include "../samplers/Sampler.hpp"
#include <memory>


namespace nyas
{
    /// Bidirectional reflectance distribution function interface.
    class BRDF
    {
    public:
        BRDF inline & set_sampler(SamplerPtr const& sampler)
        {
            this->_sampler = sampler;
            return *this;
        }

        SamplerPtr inline sampler() const
        {
            return this->_sampler;
        }

        /// return the BRDF value depends on incident and outgoing ray. For fast calculation
        /// , the return value should be 2pi times actual BRDF value.
        ///
        /// @param normal surface normal that ray hit object, it should be facing out of surface
        /// @param incident incident ray direction, it should point to surface instead of leaving
        /// @param outgoing outgoing ray direction, it should leave from surface
        float32 virtual operator()(Vector3D const& normal, Vector3D const& incident, Vector3D const& outgoing) const = 0;

        /// return outgoing ray direction meets the BRDF.
        ///
        /// @param normal surface normal that ray hit object, it should be facing out of surface
        /// @param incident incident ray direction, it should point to surface, instead of leaving
        Vector3D inline scatter(Vector3D const& normal, Vector3D const& incident) const
        {
            Vector3D const tar = Sampler::map_to_hemisphere(this->_sampler->sample_uniform2D(), 1.);
            //Vector3D const tar = Sampler::map_to_hemisphere(random::uniform2D(), 1.);
            float64 const
                r3D = length(normal),
                r2D = sqrt(normal.x * normal.x + normal.y * normal.y);
            float64 const
                cb = normal.z / r3D, sb = r2D / r3D,
                ca = normal.x / r2D, sa = normal.y / r2D;
            float64 const _f = cb * tar.y + sb * tar.z;
            return Vector3D(
                sa * tar.x + ca * _f,
                -ca * tar.x + sa * _f,
                cb * tar.z - sb * tar.y
            );
        }


    protected:
        SamplerPtr _sampler;
    };

    typedef shared_ptr<BRDF> BRDFPtr;
    typedef shared_ptr<BRDF const> BRDFConstptr;

} // namespace nyas
