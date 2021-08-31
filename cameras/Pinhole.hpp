/// @file cameras/Pinhole.hpp
#pragma once

#include "../common/types.hpp"
#include "Camera.hpp"
#include <memory>


namespace nyas
{
    namespace cameras
    {
        class Pinhole : public Camera
        {
        public:
            /* Constructors */
            Pinhole()
                : Camera()
                , _view_point(0.)
            {}
            explicit Pinhole(length_t figure_width, length_t figure_height)
                : Camera(figure_width, figure_height)
                , _view_point(0.)
            {}
            explicit Pinhole(Length2D const& figure_size)
                : Camera(figure_size)
                , _view_point(0.)
            {}
            explicit Pinhole(Length2D const& figure_size,
                             Point3D const& figure_center,
                             Vector3D const& figure_u,
                             Vector3D const& figure_v,
                             Point3D const& view_point)
                : Camera(figure_size, figure_center, figure_u, figure_v)
                , _view_point(view_point)
            {}

            void inline set_view_point(Point3D const& view_point)
            {
                _view_point = view_point;
            }

            Point3D inline view_point() const
            {
                return _view_point;
            }
            Vector3D inline view_direction() const
            {
                return _figure_center - _view_point;
            }

            /// get ray on figure in 3D-space
            ///
            /// @param p floating-point vector in range [-1, 1]
            Ray virtual inline get_ray(Point2D const& p) const override
            {
                Point3D const p3 = at(p);
                return Ray(
                    p3,
#ifdef GET_RAY_WITH_NORMALIZE
                    normalize(p3 - _view_point)
#else
                    p3 - _view_point
#endif
                );
            }

            /// get ray on figure in 3D-space
            ///
            /// @param i pixel index on figure in range [0, width] * [0, height]
            Ray virtual inline get_ray(Length2D const& i) const override
            {
                Point3D const p3 = at(i);
                return Ray(
                    p3,
#ifdef GET_RAY_WITH_NORMALIZE
                    normalize(p3 - _view_point)
#else
                    p3 - _view_point
#endif
                );
            }


        private:
            Point3D _view_point;
        };

        typedef std::shared_ptr<Pinhole> PinholePtr;
        typedef std::shared_ptr<Pinhole const> PinholeConstptr;


        /// get a default pinhole camera
        ///
        /// @param fov field of view using radian
        PinholePtr default_pinhole(Length2D const& figure_size,
                                   Point3D const& view_point,
                                   Vector3D const& view_direction,
                                   float64 fov,
                                   Vector3D const& view_up = Camera::DEFAULT_VIEW_UP)
        {
            float64 constexpr view_distance = 1.;       // looks like view_distance is useless in pinhole camera
            float64 const scalar = view_distance * tan(0.5 * fov);
            PinholePtr pinhole = std::make_shared<Pinhole>(figure_size);
            pinhole->set_view_point(view_point);
            pinhole->set_figure_center(view_point + view_distance * view_direction);
            pinhole->set_figure_direction_u(scalar * normalize(cross(view_direction, view_up)));
            pinhole->set_figure_direction_v((scalar * figure_size.y / figure_size.x) * normalize(cross(pinhole->figure_direction_u(), view_direction)));
            return pinhole;
        }

    } // namespace cameras

} // namespace nyas
