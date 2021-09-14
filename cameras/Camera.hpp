/// @file cameras/Camera.hpp
#pragma once

#include "../common/types.hpp"
#include "../common/constants.hpp"
#include "../common/functions.hpp"
#include "../samplers/Sampler.hpp"
#include "../Ray.hpp"
#include "../Buffer2D.hpp"
#include <memory>
#include <tuple>


namespace nyas
{
    using ::std::tuple;


    /// Basic Camera interface
    class Camera
    {
    public:
        Vector3D static constexpr DEFAULT_VIEW_UP = constants<float64>::axis3D::Z;
        Vector3D static constexpr APPROXIMATE_VIEW_UP = Vector3D(0.00967967, 0.000679879, 1.);


        /* Constructors */
        explicit Camera(length_t const& figure_width, length_t const& figure_height)
            : Camera(Length2D(figure_width, figure_height))
        {}
        explicit Camera(Length2D const& figure_size)
            : Camera(
                figure_size,
                constants<float64>::axis3D::O,
                constants<float64>::axis3D::O,
                constants<float64>::axis3D::O,
                nullptr
            )
        {}
        explicit Camera(
            Length2D const& figure_size,
            Point3D const& figure_center,
            Vector3D const& figure_u,
            Vector3D const& figure_v,
            SamplerPtr const& sampler
        )
            : _figure(figure_size)
            , _figure_center(figure_center)
            , _figure_u(figure_u)
            , _figure_v(figure_v)
            , _sampler(sampler)
        {
#ifdef REDUCE_POINT_BEYOND_RANGE
            this->_inverse_figure_size = 1. / Point2D(this->_figure.size());
#endif
        }

        /* Destructor */
        virtual ~Camera() = default;


        bool virtual inline valid() const
        {
            return this->_figure.valid() && !(near_to_zero(this->_figure_u) || near_to_zero(this->_figure_v));
        }

        Camera inline & set_figure_direction_u(Vector3D const& u)
        {
            this->_figure_u = u;
            return *this;

        }
        Camera inline & set_figure_direction_v(Vector3D const& v)
        {
            this->_figure_v = v;
            return *this;
        }
        Camera inline & set_figure_directions(Vector3D const& u, Vector3D const& v)
        {
            this->_figure_u = u;
            this->_figure_v = v;
            return *this;
        }
        Camera inline & set_figure_center(Point3D const& c)
        {
            this->_figure_center = c;
            return *this;
        }
        Camera inline & set_sampler(SamplerPtr const& sampler)
        {
            this->_sampler = sampler;
            return *this;
        }

        Length2D inline figure_size() const
        {
            return this->_figure.size();
        }
        Point3D inline figure_center() const
        {
            return this->_figure_center;
        }
        Vector3D inline figure_direction_u() const
        {
            return this->_figure_u;
        }
        Vector3D inline figure_direction_v() const
        {
            return this->_figure_v;
        }
        tuple<Vector3D, Vector3D> inline figure_directions() const
        {
            return ::std::make_tuple(this->_figure_u, this->_figure_v);
        }
        GraphicsBuffer inline & figure()
        {
            return this->_figure;
        }
        GraphicsBuffer inline const& figure() const
        {
            return this->_figure;
        }
        SamplerPtr inline sampler() const
        {
            return this->_sampler;
        }

        /// get point position on figure in 3D-space
        ///
        /// @param p floating-point vector in range [-1, 1]^2
        Point3D inline at(Point2D const& p) const
        {
#ifdef REDUCE_POINT_BEYOND_RANGE
            Point2D const reduce = reduce_over01(p * 0.5 + 0.5) * 2. - 1.;
            return this->_figure_center + reduce.x * this->_figure_u + reduce.y * this->_figure_v;
#else
            return this->_figure_center + p.x * this->_figure_u + p.y * this->_figure_v;
#endif
        }

        /// get point position on figure in 3D-space
        ///
        /// @param i pixel index on figure in range [0, width] * [0, height]
        Point3D inline at(Length2D const& i) const
        {
#ifdef REDUCE_POINT_BEYOND_RANGE
            Point2D p = reduce_over01(Point2D(i) * this->_inverse_figure_size) * 2. - 1.;
#else
            Point2D p = Point2D(i) * this->_inverse_figure_size * 2. - 1.;
#endif
            return this->_figure_center + p.x * this->_figure_u + p.y * this->_figure_v;
        }

        /// get ray on figure in 3D-space
        ///
        /// @param p floating-point vector in range [-1, 1]^2
        Ray virtual inline get_ray(Point2D const& p) const = 0;

        /// get ray on figure in 3D-space
        ///
        /// @param p pixel index on figure in range [0, width] * [0, height]
        Ray virtual inline get_ray(Length2D const& p) const = 0;

        Ray virtual get_ray_sample(Length2D const& p) const = 0;


    protected:
        GraphicsBuffer _figure;
        Point3D _figure_center;
        Vector3D _figure_u;
        Vector3D _figure_v;
        Point2D _inverse_figure_size;
        SamplerPtr _sampler;
    };

    typedef shared_ptr<Camera> CameraPtr;
    typedef shared_ptr<Camera const> CameraConstptr;


    /// return correct view_direction and view_up with azimuth, zenith angle and tilt angle
    ///
    /// @param azimuth angle between projection of view_direction on xOy plane and x axis
    /// @param zenith_angle angle between view_direction and z axis
    /// @param tilt_angle angle at camera rotates around view_direction
    tuple<Vector3D, Vector3D> inline correct_view(
        float64 const& azimuth,
        float64 const& zenith_angle,
        float64 const& tilt_angle
    )
    {
        float64 const sa = sin(azimuth), ca = cos(azimuth);
        float64 const sz = sin(zenith_angle), cz = cos(zenith_angle);
        float64 const st = sin(tilt_angle), ct = cos(tilt_angle);
        return ::std::make_tuple(
            Vector3D(ca * sz, sa * sz, cz),
            Vector3D(
                sa * st - ca * cz * ct,
                -ca * st - sa * cz * ct,
                sz * ct
            )
        );
    }

} // namespace nyas
