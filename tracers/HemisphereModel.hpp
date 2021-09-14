/// @file tracer/HemisphereModel.hpp
#pragma once

#include "RayTracer.hpp"
#include "../common/constants.hpp"
#include "../common/functions.hpp"
#include "../brdfs/BRDF.hpp"
#include "../objects/Object3D.hpp"
#include "iostream"


namespace nyas
{
    namespace tracers
    {
        class HemisphereModel final : public RayTracer
        {
        public:
            HemisphereModel()
                : RayTracer()
            {}
            explicit HemisphereModel(length_t const& max_steps)
                : RayTracer(max_steps)
            {}
            explicit HemisphereModel(length_t const& max_steps, World const* const& world)
                : RayTracer(max_steps, world)
            {}

            RGBColor virtual trace_ray(Ray const& ray) const override
            {
                return this->_trace_ray_step(ray, this->_max_steps);
            }


        private:
            RGBColor _trace_ray_step(Ray const& ray, length_t const& step) const
            {
                if (step <= 0) {
                    return constants<float32>::axis3D::O;
                }
                RayHittingRecord rec;
                bool hit_anything = false;
                for (Object3DConstptr const& obj : this->_world->objects()) {
                    hit_anything |= obj->hit(ray, rec.t, rec);
                }
                if (hit_anything) {
                    BRDF const& brdf = *rec.object->BRDF();
                    Vector3D normal = (dot(rec.normal, ray.direction) < 0) ? rec.normal : -rec.normal;
                    Ray scattered_ray(rec.hitting_point, brdf.scatter(normal, ray.direction));
                    return
                        //TODO: rec.object->light +
                            (//TODO: rec.object->texture *
                            brdf(normal, ray.direction, scattered_ray.direction) *
                            static_cast<float32>(dot(normal, scattered_ray.direction))) *
                            this->_trace_ray_step(scattered_ray, step - 1);
                }
                return this->_world->sky()->get_color(ray.direction);
            }
        };

        typedef shared_ptr<HemisphereModel> HemisphereModelPtr;
        typedef shared_ptr<HemisphereModel const> HemisphereModelConstpstr;

    } // namespace traces

} // namespace nyas
