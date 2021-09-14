/// @file cameras/Pinhole.hpp
#pragma once

#include "Camera.hpp"
#include "../common/types.hpp"
#include <memory>


namespace nyas
{
    namespace cameras
    {
        class Pinhole : public Camera
        {
        public:
            /* Constructors */
            explicit Pinhole(length_t const& figure_width, length_t const& figure_height)
                : Camera(figure_width, figure_height)
                , _view_point(0.)
            {}
            explicit Pinhole(Length2D const& figure_size)
                : Camera(figure_size)
                , _view_point(0.)
            {}
            explicit Pinhole(
                Length2D const& figure_size,
                Point3D const& figure_center,
                Vector3D const& figure_u,
                Vector3D const& figure_v,
                SamplerPtr const& sampler,
                Point3D const& view_point
            )
                : Camera(figure_size, figure_center, figure_u, figure_v, sampler)
                , _view_point(view_point)
            {}

            bool virtual inline valid() const override
            {
                return Camera::valid() && !near_to_zero(this->view_direction());
            }

            Pinhole inline & set_view_point(Point3D const& view_point)
            {
                this->_view_point = view_point;
                return *this;
            }

            Point3D inline view_point() const
            {
                return this->_view_point;
            }
            Vector3D inline view_direction() const
            {
                return this->_figure_center - this->_view_point;
            }

            /// get ray on figure in 3D-space
            ///
            /// @param p floating-point vector in range [-1, 1]
            Ray virtual inline get_ray(Point2D const& p) const override
            {
                Point3D const p3 = this->at(p);
                return Ray(p3, p3 - this->_view_point);
            }

            /// get ray on figure in 3D-space
            ///
            /// @param i pixel index on figure in range [0, width] * [0, height]
            Ray virtual inline get_ray(Length2D const& i) const override
            {
                Point3D const p3 = this->at(i);
                return Ray(p3, p3 - this->_view_point);
            }

            Ray virtual get_ray_sample(Length2D const& i) const override
            {
                Point3D const p3 = this->at((Point2D(i) + this->_sampler->sample_uniform2D()) * this->_inverse_figure_size * 2. - 1.);
                return Ray(p3, p3 - this->_view_point);
            }


        private:
            Point3D _view_point;
        };

        typedef shared_ptr<Pinhole> PinholePtr;
        typedef shared_ptr<Pinhole const> PinholeConstptr;


        /// get a default pinhole camera
        ///
        /// @param fov field of view using radian
        PinholePtr default_pinhole(
            Length2D const& figure_size,
            Point3D const& view_point,
            Vector3D const& view_direction,
            float64 const& fov,
            Vector3D const& view_up = Camera::DEFAULT_VIEW_UP
        )
        {
            float64 constexpr view_distance = 1.;       // looks like view_distance is useless in pinhole camera
            float64 const scalar = view_distance * tan(0.5 * fov);
            PinholePtr pinhole = make_shared<Pinhole>(figure_size);
            pinhole->set_view_point(view_point);
            pinhole->set_figure_center(view_point + view_distance * normalize(view_direction));
            pinhole->set_figure_direction_u(scalar * normalize(cross(view_direction, view_up)));
            pinhole->set_figure_direction_v((scalar * figure_size.y / figure_size.x) * normalize(cross(pinhole->figure_direction_u(), view_direction)));
            return pinhole;
        }

    } // namespace cameras

} // namespace nyas
