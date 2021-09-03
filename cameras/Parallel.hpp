/// @file cameras/Parallel.hpp
#pragma once

#include "../common/types.hpp"
#include "Camera.hpp"
#include <memory>


namespace nyas
{
    namespace cameras
    {
        class Parallel : public Camera
        {
        public:
            /* Constructors */
            Parallel()
                : Camera()
                , _view_direction(0.)
            {}
            explicit Parallel(length_t figure_width, length_t figure_height)
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
                Vector3D const& view_direction
            )
                : Camera(figure_size, figure_center, figure_u, figure_v)
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
                return Ray(
                    this->at(p),
#ifdef GET_RAY_WITH_NORMALIZE
                    normalize(this->_view_direction)
#else
                    this->_view_direction
#endif
                );
            }

            /// get ray on figure in 3D-space
            ///
            /// @param i pixel index on figure in range [0, width] * [0, height]
            Ray virtual inline get_ray(Length2D const& i) const override
            {
                return Ray(
                    this->at(i),
#ifdef GET_RAY_WITH_NORMALIZE
                    normalize(this->_view_direction)
#else
                    this->_view_direction
#endif
                );
            }


        private:
            Vector3D _view_direction;
        };

        typedef std::shared_ptr<Parallel> ParallelPtr;
        typedef std::shared_ptr<Parallel const> ParallelConstptr;


        /// get a default patallel camera
        ///
        /// @param figure_width_scalar length of figure width in 3D-space
        ParallelPtr default_parallel(
            Length2D const& figure_size,
            float64 figure_width_scalar,
            Point3D const& figure_center,
            Vector3D const& view_direction,
            Vector3D const& view_up = Camera::DEFAULT_VIEW_UP
        )
        {
            float64 const half_scalar = 0.5 * figure_width_scalar;
            ParallelPtr parallel = std::make_shared<Parallel>(figure_size);
            parallel->set_figure_center(figure_center);
            parallel->set_view_direction(view_direction);
            parallel->set_figure_direction_u(half_scalar * normalize(cross(view_direction, view_up)));
            parallel->set_figure_direction_v((half_scalar * figure_size.y / figure_size.x) * normalize(cross(parallel->figure_direction_u(), view_direction)));
            return parallel;
        }

    } // namespace cameras

} // namespace nyas

