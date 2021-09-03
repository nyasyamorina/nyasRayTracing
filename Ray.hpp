/// @file Ray.hpp
#pragma once

#include "common/types.hpp"


namespace nyas
{
    struct Ray
    {
        Point3D origin;
        Vector3D direction;


        /* Constructors */
        Ray() = default;
        explicit Ray(Point3D const& o, Vector3D const& d)
            : origin(o)
            , direction(d)
        {}
        Ray(Ray const&) = default;

        Ray & operator=(Ray const&) = default;

        /// return ray position by passing time t
        Point3D inline at(float64 t) const
        {
            return this->origin + this->direction * t;
        }
    };

} // namespace nyas
