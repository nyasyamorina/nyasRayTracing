/// @file objects/Object3D.hpp
#pragma once

#include "../common/types.hpp"
#include "../Ray.hpp"
#include "../brdfs/BRDF.hpp"
#include <memory>


namespace nyas
{
    class Object3D
    {
    public:
        bool virtual is_hit(Ray const& ray, float64 t_max) const = 0;


    protected:
        BRDFPtr _brdf;
    };

    typedef ::std::shared_ptr<Object3D> Object3DPtr;
    typedef ::std::shared_ptr<Object3D const> Object3DConstptr;

} // namespace nyas
