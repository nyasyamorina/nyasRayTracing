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
    /// @param side side length of sampler grid
    class Sampler
    {
    public:
        typedef std::vector<Point2D> SampleList;


        /// mapping a 2D point into r=1 2D-disk (unit-circle)
        ///
        /// @param p point in unit-square (range [0, 1]^2)
        Point2D static map_to_circle(Point2D p)
        {
            p *= 2.; p -= 1.;
            if (near_to_zero(p)) {
                return Point2D(0.);
            }
            float64 constexpr pi_over_4 = quarter_pi<float64>();
            float64 r, phi;
            if (p.x > -p.y) {
                if (p.x > p.y) {
                    r = p.x;
                    phi = p.y / p.x;
                }
                else {
                    r = p.y;
                    phi = 2. - p.x / p.y;
                }
            }
            else {
                if (p.x < p.y) {
                    r = -p.x;
                    phi = 4. + p.y / p.x;
                }
                else {
                    r = -p.y;
                    phi = 6. - p.x / p.y;
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
            float64 constexpr tau = two_pi<float64>();
            float64 const cos_phi = cos(tau * p.x);
            float64 const sin_phi = sin(tau * p.x);
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

        Sampler(length_t const& side)
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

