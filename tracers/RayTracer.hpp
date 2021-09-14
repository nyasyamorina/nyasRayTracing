/// @file tracers/RayTracer.hpp
#pragma once

#include "../common/types.hpp"
#include "../Ray.hpp"


namespace nyas
{
    class World;

    class RayTracer
    {
    public:
        RayTracer()
            : _max_steps(0)
            , _world(nullptr)
        {}
        explicit RayTracer(length_t const& max_steps)
            : _max_steps(max_steps)
            , _world(nullptr)
        {}
        explicit RayTracer(length_t const& max_steps, World const* const& world)
            : _max_steps(max_steps)
            , _world(world)
        {}

        RayTracer inline & set_max_steps(length_t const& max_steps)
        {
            this->_max_steps = max_steps;
            return *this;
        }
        RayTracer inline & set_world(World const* const& world)
        {
            this->_world = world;
            return *this;
        }

        length_t inline max_steps() const
        {
            return this->_max_steps;
        }
        World inline const* world() const
        {
            return this->_world;
        }

        RGBColor virtual trace_ray(Ray const& ray) const = 0;


    protected:
        length_t _max_steps;
        World const* _world;
    };

    typedef shared_ptr<RayTracer> RayTracerPtr;
    typedef shared_ptr<RayTracer const> RayTracerConstptr;

} // namespace nyas

#include "../World.hpp"
