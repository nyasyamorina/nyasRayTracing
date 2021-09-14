/// @file cameras/Parallel.hpp
#pragma once

#include "Camera.hpp"
#include "../common/types.hpp"
#include <memory>


namespace nyas
{
    namespace cameras
    {
        class Parallel : public Camera
        {
        public:
            /* Constructors */
            explicit Parallel(length_t const& figure_width, length_t const& figure_height)
                : Camera(figure_width, figure_height)
                , _view_direction(0.)
            {}
            explicit Parallel(Length2D const& figure_size)
                : Camera(figure_size)
                , _view_direction(0.)
            {}
            explicit Parallel(
                Length2D const& figure_size,
                Point3D const& figure_center,
                Vector3D const& figure_u,
                Vector3D const& figure_v,
                SamplerPtr const& sampler,
                Vector3D const& view_direction
            )
                : Camera(figure_size, figure_center, figure_u, figure_v, sampler)
                , _view_direction(view_direction)
            {}

            bool virtual inline valid() const override
            {
                return Camera::valid() && !near_to_zero(this->_view_direction);
            }

            Parallel inline & set_view_direction(Vector3D const& d)
            {
                this->_view_direction = d;
                return *this;
            }

            Vector3D inline view_direction() const
            {
                return this->_view_direction;
            }

            /// get ray on figure in 3D-space
            ///
            /// @param p floating-point vector in range [-1, 1]
            Ray virtual inline get_ray(Point2D const& p) const override
            {
                return Ray(this->at(p), this->_view_direction);
            }

            /// get ray on figure in 3D-space
            ///
            /// @param i pixel index on figure in range [0, width] * [0, height]
            Ray virtual inline get_ray(Length2D const& i) const override
            {
                return Ray(this->at(i), this->_view_direction);
            }

            Ray virtual get_ray_sample(Length2D const& i) const override
            {
                return Ray(
                    this->at((Point2D(i) + this->_sampler->sample_uniform2D()) * this->_inverse_figure_size * 2. - 1.),
                    this->_view_direction
                );
            }


        private:
            Vector3D _view_direction;
        };

        typedef shared_ptr<Parallel> ParallelPtr;
        typedef shared_ptr<Parallel const> ParallelConstptr;


        /// get a default parallel camera
        ///
        /// @param figure_width_scalar length of figure width in 3D-space
        ParallelPtr default_parallel(
            Length2D const& figure_size,
            float64 const& figure_width_scalar,
            Point3D const& figure_center,
            Vector3D const& view_direction,
            Vector3D const& view_up = Camera::DEFAULT_VIEW_UP
        )
        {
            float64 const half_scalar = 0.5 * figure_width_scalar;
            ParallelPtr parallel = make_shared<Parallel>(figure_size);
            parallel->set_figure_center(figure_center);
            parallel->set_view_direction(view_direction);
            parallel->set_figure_direction_u(half_scalar * normalize(cross(view_direction, view_up)));
            parallel->set_figure_direction_v((half_scalar * figure_size.y / figure_size.x) * normalize(cross(parallel->figure_direction_u(), view_direction)));
            return parallel;
        }

    } // namespace cameras

} // namespace nyas

