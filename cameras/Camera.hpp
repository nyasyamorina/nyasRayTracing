/// @file cameras/Camera.hpp
#pragma once

#include "../common/types.hpp"
#include "../common/functions.hpp"
#include "../Ray.hpp"
#include "../GraphicsBuffer.hpp"
#include <memory>


#define REDUCE_POINT_CROSS_BORDER
//#define GET_RAY_WITH_NORMALIZE


namespace nyas
{
    /// Basic Camera interface
    class Camera
    {
    public:
        typedef GraphicsBuffer<float32> Figure;

        Vector3D static constexpr DEFAULT_VIEW_UP = axis3D<float64>::Z();
        Vector3D static constexpr APPROXIMATE_VIEW_UP = Vector3D(0.00967967, 0.000679879, 1.);


        /* Constructors */
        Camera()
            : Camera(Length2D(1))
        {}
        explicit Camera(length_t figure_width, length_t figure_height)
            : Camera(Length2D(figure_width, figure_height))
        {}
        explicit Camera(Length2D const& figure_size)
            : _figure(figure_size)
            , _figure_center(0.)
            , _figure_u(0.)
            , _figure_v(0.)
        {}
        explicit Camera(Length2D const& figure_size,
                        Point3D const& figure_center,
                        Vector3D const& figure_u,
                        Vector3D const& figure_v)
            : _figure(figure_size)
            , _figure_center(figure_center)
            , _figure_u(figure_u)
            , _figure_v(figure_v)
        {}


        bool virtual inline valid() const
        {
            return _figure.valid() && !(near_to_zero(_figure_u) || near_to_zero(_figure_v));
        }

        void inline set_figure_direction_u(Vector3D const& u)
        {
            _figure_u = u;
        }
        void inline set_figure_direction_v(Vector3D const& v)
        {
            _figure_v = v;
        }
        void inline set_figure_directions(Vector3D const& u, Vector3D const& v)
        {
            _figure_u = u;
            _figure_v = v;
        }
        void inline set_figure_center(Point3D const& c)
        {
            _figure_center = c;
        }

        Length2D inline figure_size() const
        {
            return _figure.size();
        }
        Point3D inline figure_center() const
        {
            return _figure_center;
        }
        Vector3D inline figure_direction_u() const
        {
            return _figure_u;
        }
        Vector3D inline figure_direction_v() const
        {
            return _figure_v;
        }
        Figure inline & figure()
        {
            return _figure;
        }
        Figure inline const& figure() const
        {
            return _figure;
        }

        /// get point position on figure in 3D-space
        ///
        /// @param p floating-point vector in range [-1, 1]^2
        Point3D inline at(Point2D const& p) const
        {
#ifdef REDUCE_POINT_CROSS_BORDER
            Point2D const reduce = reduce_over01(p * 0.5 + 0.5) * 2. - 1.;
            return _figure_center + reduce.x * _figure_u + reduce.y * _figure_v;
#else
            return _figure_center + p.x * _figure_u + p.y * _figure_v;
#endif
        }

        /// get point position on figure in 3D-space
        ///
        /// @param i pixel index on figure in range [0, width] * [0, height]
        Point3D inline at(Length2D const& i) const
        {
            Point2D const static inverse_size = 1. / Point2D(_figure.size());
#ifdef REDUCE_POINT_CROSS_BORDER
            Point2D p = reduce_over01(Point2D(i) * inverse_size) * 2. - 1.;
#else
            Point2D p = Point2D(i) * inverse_size * 2. - 1.;
#endif
            return _figure_center + p.x * _figure_u + p.y * _figure_v;
        }

        /// get pixel index on figure
        ///
        /// @param p floating-point vector in range [-1, 1]^2
        Length2D inline on_figure(Point2D const& p) const
        {
            Point2D const static size = Point2D(_figure.size());
#ifdef REDUCE_POINT_CROSS_BORDER
            return Length2D(reduce_over01(p * 0.5 + 0.5) * size);
#else
            return Length2D((p * 0.5 + 0.5) * size);
#endif
        }

        /// get point position on figure in 2D-space
        ///
        /// @param i pixel index on figure in range [0, width] * [0, height]
        Point2D inline on_figure(Length2D const& i) const
        {
            Point2D const static inverse_size = 1. / Point2D(_figure.size());
#ifdef REDUCE_POINT_CROSS_BORDER
            return reduce_over01(Point2D(i) * inverse_size) * 2. - 1.;
#else
            return Point2D(i) * inverse_size * 2. - 1.;
#endif
        }

        /// get ray on figure in 3D-space
        ///
        /// @param p floating-point vector in range [-1, 1]^2
        Ray virtual inline get_ray(Point2D const& p) const = 0;

        /// get ray on figure in 3D-space
        ///
        /// @param p pixel index on figure in range [0, width] * [0, height]
        Ray virtual inline get_ray(Length2D const& p) const = 0;


    protected:
        Figure _figure;
        Point3D _figure_center;
        Vector3D _figure_u;
        Vector3D _figure_v;
    };

    typedef std::shared_ptr<Camera> CameraPtr;
    typedef std::shared_ptr<Camera const> CameraConstptr;


    /// set correct view_direction and return correct view_up with azimuth, zenith angle and tilt angle
    ///
    /// @param azimuth angle between projection of view_direction on xOy plane and x axis
    /// @param zenith_angle angle between view_direction and z axis
    /// @param tilt_angle angle at camera rotates around view_direction
    /// @param view_direction view_direction output
    Vector3D inline correct_view(float64 azimuth, float64 zenith_angle, float64 tilt_angle,
                                 Vector3D & view_direction)
    {
        float64 sa = sin(azimuth), ca = cos(azimuth);
        float64 sz = sin(zenith_angle), cz = cos(zenith_angle);
        float64 st = sin(tilt_angle), ct = cos(tilt_angle);
        view_direction.x = ca * sz;
        view_direction.y = sa * sz;
        view_direction.z = cz;
        return Vector3D(
            sa * st - ca * cz * ct,
            -ca * st - sa * cz * ct,
            sz * ct
        );
    }

} // namespace nyas
