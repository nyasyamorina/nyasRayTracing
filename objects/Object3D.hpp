/// @file objects/Object3D.hpp
#pragma once

#include "../common/types.hpp"
#include "../common/constants.hpp"
#include "../samplers/Sampler.hpp"
#include "../Ray.hpp"
#include "../brdfs/BRDF.hpp"
#include <memory>


namespace nyas
{
    class Object3D;


    struct RayHittingRecord final
    {
        float64 t;              // how far ray pass (actual distance = length(ray.direction) * t)
        Point3D hitting_point;  // where ray hit, i.e., ray.position + t * ray.direction
        Vector3D normal;        // surface normal that ray hit, it should be facing out of surface
        Object3D const* object; // what object ray hit


        RayHittingRecord()
            : t(constants<float64>::infinity)
            , hitting_point(constants<float64>::infinity)
            , normal(0.)
            , object(nullptr)
        {}
    };



    class Object3D
    {
    public:
        Object3D()
            : _brdf(nullptr)
        {}
        explicit Object3D(BRDFPtr brdf)
            : _brdf(brdf)
        {}

        Object3D inline & set_BRDF(BRDFPtr const& brdf)
        {
            _brdf = brdf;
            return *this;
        }
        Object3D inline & set_sampler(SamplerPtr const& sampler)
        {
            this->_sampler = sampler;
            if (this->_brdf != nullptr) {
                this->_brdf->set_sampler(sampler);
            }
            return *this;
        }

        BRDFPtr inline BRDF() const
        {
            return this->_brdf;
        }
        SamplerPtr inline sampler() const
        {
            return this->_sampler;
        }

        bool virtual hit(Ray const& ray, float64 const& t_max, RayHittingRecord & rec) const = 0;


    protected:
        BRDFPtr _brdf;
        SamplerPtr _sampler;
    };

    typedef shared_ptr<Object3D> Object3DPtr;
    typedef shared_ptr<Object3D const> Object3DConstptr;

} // namespace nyas
