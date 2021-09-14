/// @file samplers/Sampler.hpp
#pragma once

#include "SamplesGenerator.hpp"
#include "../common/constants.hpp"
#include "../common/functions.hpp"
#include "../common/randoms.hpp"
#include <assert.h>


namespace nyas
{
    class Sampler final
    {
    public:
        /// mapping a 2D point into r=1 2D-disk (unit-circle)
        ///
        /// @param p point in unit-square (range [0, 1]^2)
        Point2D static map_to_circle(Point2D p)
        {
            p *= 2.; p -= 1.;
            if (near_to_zero(p)) {
                return constants<float64>::axis2D::O;
            }
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
            phi *= constants<float64>::pi_over_four;
            return r * Point2D(cos(phi), sin(phi));
        }

        /// mapping a 2D point into 3D-unit-hemisphere (r=1, z>=0) with zenith density value e
        ///
        /// @param p point in unit-square (range [0, 1]^2)
        Point3D static map_to_hemisphere(Point2D const& p, float64 const& e)
        {
            float64 const cos_phi = cos(constants<float64>::two_pi * p.x);
            float64 const sin_phi = sin(constants<float64>::two_pi * p.x);
            float64 const cos_theta = pow(1. - p.y, 1. / (e + 1.));
            float64 const sin_theta = sqrt(1. - cos_theta * cos_theta);
            return Point3D(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
        }


        explicit Sampler(SamplesGenerator const& generator)
            : _num_sets(generator.num_sets())
            , _num_samples(generator.num_samples())
            , _ele_count(0)
            //, _set_count(0)
        {
            assert(generator.num_sets() > 0 && generator.num_samples() > 0);
            if (this->_num_samples == 1) {
                this->_num_sets = 1;
                this->_samples.push_back(Point2D(0.5));
            }
            else {
                this->_samples = generator.generate_samples();
            }
            this->_num_total = this->_samples.size();
            assert(this->_num_sets * this->_num_samples == this->_num_total);
        }

        length_t inline num_sets() const
        {
            return this->_num_sets;
        }
        length_t inline num_samples() const
        {
            return this->_num_samples;
        }
        length_t inline num_total() const
        {
            return this->_num_total;
        }
        SampleList inline const& samples() const
        {
            return this->_samples;
        }

        Point2D inline sample_uniform2D()
        {
            return this->_samples[(this->_ele_count++) % this->_num_total];
            //if (this->_ele_count % this->_num_samples == 0) {
            //    this->_set_count = (random::integer() % this->_num_sets) * this->_num_samples;
            //}
            //return this->_samples[(this->_ele_count++) % this->_num_samples + this->_set_count];
        }


    private:
        length_t _num_sets;
        length_t _num_samples;
        length_t _num_total;
        length_t _ele_count;
        //length_t _set_count;    // randomly selectee sample set
        SampleList _samples;
    };

    typedef shared_ptr<Sampler> SamplerPtr;
    typedef shared_ptr<Sampler const> SamplerConstptr;

} // namespace nyas
