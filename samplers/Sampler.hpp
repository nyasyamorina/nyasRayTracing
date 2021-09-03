/// @file samplers/Sampler.hpp
#pragma once

#include "../common/types.hpp"
#include "../common/constants.hpp"
#include "../common/functions.hpp"
#include <vector>


namespace nyas
{
    /// Take multi samples in single pixel.
    /// number of samples should be perfect square.
    ///
    /// @param n side length of sampler grid
    class Sampler
    {
    public:
        typedef std::vector<Point2D> SampleList;


        /// mapping a 2D point into r=1 2D-disk (unit-circle)
        ///
        /// @param p point in unit-square (range [0, 1]^2)
        Point2D static map_to_circle(Point2D const& p)
        {
            Point2D const point = p * 2. - 1.;
            if (near_to_zero(point)) {
                return Point2D(0.);
            }
            float64 constexpr pi_over_4 = quarter_pi<float64>();
            float64 r, phi;
            if (point.x > -point.y) {
                if (point.x > point.y) {
                    r = point.x;
                    phi = point.y / point.x;
                }
                else {
                    r = point.y;
                    phi = 2. - point.x / point.y;
                }
            }
            else {
                if (point.x < point.y) {
                    r = -point.x;
                    phi = 4. + point.y / point.x;
                }
                else {
                    r = -point.y;
                    phi = 6. - point.x / point.y;
                }
            }
            phi *= pi_over_4;
            return r * Point2D(cos(phi), sin(phi));
        }

        /// mapping a 2D point into 3D-unit-hemisphere (r=1, z>=0) with zenith density value e
        ///
        /// @param p point in unit-square (range [0, 1]^2)
        Point3D static map_to_hemisphere(Point2D const& p, float64 const& e)
        {
            float64 constexpr two_pi = ::nyas::two_pi<float64>();
            float64 const cos_phi = cos(two_pi * p.x);
            float64 const sin_phi = sin(two_pi * p.x);
            float64 const cos_theta = pow(1. - p.y, 1. / (e + 1.));
            float64 const sin_theta = sqrt(1. - cos_theta * cos_theta);
            return Point3D(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
        }


        length_t inline side_length() const
        {
            return _side_length;
        }
        length_t inline num_samples() const
        {
            return _num_samples;
        }

        SampleList inline & samples()
        {
            return _samples;
        }
        SampleList inline const& samples() const
        {
            return _samples;
        }

        /// Return next sample
        Point2D next()
        {
            Point2D next = _samples[_count++];
            if (_count >= _num_samples) {
                _count = 0;
            }
            return next;
        }


    protected:
        length_t _side_length;
        length_t _num_samples;
        SampleList _samples;
        length_t _count;

        Sampler(length_t side)
            : _side_length(side)
            , _num_samples(side * side)
            , _count(0)
        {
            _samples.reserve(_num_samples);
        }

        /// Generate samples in range [0, 1]^2
        void virtual _generate_samples() = 0;
    };

} // namespace nyas

