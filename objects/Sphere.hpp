/// @file objects/Sphere.hpp
#pragma once

#include "Object3D.hpp"
#include "../common/constants.hpp"
#include "../common/functions.hpp"


namespace nyas
{
    namespace objects
    {
        class Sphere final : public Object3D
        {
        public:
            Sphere()
                : Object3D()
                , _radius(0.)
                , _center(0.)
            {}
            explicit Sphere(float64 const& radius, Point3D const& center)
                : Object3D()
                , _radius(radius)
                , _center(center)
            {}
            explicit Sphere(BRDFPtr brdf, float64 const& radius, Point3D const& center)
                : Object3D(brdf)
                , _radius(radius)
                , _center(center)
            {}

            Sphere inline & set_radius(float64 const& radius)
            {
                this->_radius = radius;
                return *this;
            }
            Sphere inline & set_center(Point3D const& center)
            {
                this->_center = center;
                return *this;
            }

            float64 inline radius() const
            {
                return this->_radius;
            }
            Point3D inline center() const
            {
                return this->_center;
            }

            bool virtual hit(Ray const& ray, float64 const& t_max, RayHittingRecord & rec) const override
            {
                // get time that ray hit sphere using quadratic equation
                Vector3D const c2o = ray.origin - this->_center;
                float64 const a =  length2(ray.direction);
                float64 const half_b = dot(ray.direction, c2o);
                float64 disc = half_b * half_b - a * (length2(c2o) - this->_radius * this->_radius);
                if (disc < 0.) {    // may ray hit sphere ?
                    return false;
                }
                disc = sqrt(disc);
                float64 t = (-disc - half_b) / a;
                if (t < 0.) {       // sphere is not in front of ray ?
                    t = (disc - half_b) / a;
                    if (t < 0.) {   // is sphere behind ray ?
                        return false;
                    }
                    // else: ray.origin in sphere
                }
                // is not sphere closer to ray.origin than other objects?
                if (t > t_max) {
                    return false;
                }
                // write sphere data into record
                rec.t = t;
                rec.hitting_point = ray.at(t);
                rec.normal = (this->_center - rec.hitting_point) * (1. / this->_radius);
                rec.object = this;
                return true;
            }


        private:
            float64 _radius;
            Point3D _center;
        };

        typedef shared_ptr<Sphere> SpherePtr;
        typedef shared_ptr<Sphere const> SphereConstptr;

    } // namespace objects

} // namespace nyas
