/// @file World.hpp
#pragma once

#include "common/types.hpp"
#include "common/constants.hpp"
#include "samplers/Sampler.hpp"
#include "cameras/Camera.hpp"
#include "objects/Object3D.hpp"
//#include "objects/MultiObject3D.hpp"
#include "skies/Sky.hpp"
#include "tracers/RayTracer.hpp"
#include <memory>
#include <vector>


namespace nyas
{
    typedef ::std::vector<Object3DPtr> Object3DList;

    class World final
    {
    public:
        World()
            : _objects()
            , _sky(nullptr)
            , _camera(nullptr)
            , _sampler(nullptr)
            , _tracer(nullptr)
        {}

        bool valid() const
        {
            for (Object3DPtr const& obj : this->_objects) {
                if (obj == nullptr || obj->BRDF() == nullptr) {
                    return false;
                }
            }
            return this->_camera != nullptr && this->_sky != nullptr;
        }

        World inline & add_object(Object3DPtr const& obj)
        {
            //MultiObject3DPtr objs = ::std::dynamic_pointer_cast<MultiObject3D>(obj);
            //if (objs == nullptr) {      // obj is not MultiObject3D
                this->_objects.push_back(obj);
            //}
            //else {                      // obj is MultiObject3D
            //    for (Object3DPtr const& subobj : objs->list()) {
            //        this->add_object(subobj);
            //    }
            //}
            return *this;
        }

        World inline & set_sky(SkyPtr const& sky)
        {
            this->_sky = sky;
            return *this;
        }
        World inline & set_camera(CameraPtr const& camera)
        {
            this->_camera = camera;
            return *this;
        }
        World inline & set_sampler(SamplerPtr const& sampler)
        {
            this->_sampler = sampler;
            for (Object3DPtr const& obj : this->_objects) {
                obj->set_sampler(sampler);
            }
            this->_camera->set_sampler(sampler);
            return *this;
        }
        World inline & set_ray_tracer(RayTracerPtr const& ray_tracer)
        {
            this->_tracer = ray_tracer;
            this->_tracer->set_world(this);
            return *this;
        }

        Object3DList inline & objects()
        {
            return this->_objects;
        }
        Object3DList inline const& objects() const
        {
            return this->_objects;
        }
        SkyPtr inline sky() const
        {
            return this->_sky;
        }
        CameraPtr inline camera() const
        {
            return this->_camera;
        }
        SamplerPtr inline sampler() const
        {
            return this->_sampler;
        }
        RayTracerPtr inline ray_tracer() const
        {
            return this->_tracer;
        }

        void render_scenes()
        {
            if(!this->valid()) {
                return;
            }
            float32 const inverse_num_samples = 1.f / this->_sampler->num_samples();
            for (length_t y = 0; y < this->_camera->figure_size().y; ++y) {
                for (length_t x = 0; x < this->_camera->figure_size().x; ++x) {
                    RGBColor pixel_color = constants<float32>::axis3D::O;
                    for (length_t n = 0; n < this->_sampler->num_samples(); ++n) {
                        pixel_color += this->_tracer->trace_ray(this->_camera->get_ray_sample(Length2D(x, y)));
                    }
                    this->_camera->figure()(x, y) = pixel_color * inverse_num_samples;
                }
            }
        }


    private:
        Object3DList _objects;
        SkyPtr _sky;
        CameraPtr _camera;
        SamplerPtr _sampler;
        RayTracerPtr _tracer;
    };

    typedef shared_ptr<World> WorldPtr;
    typedef shared_ptr<World const> WorldConstptr;

} // namespace nyas
