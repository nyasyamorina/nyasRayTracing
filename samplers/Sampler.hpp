/// @file samplers/Sampler.hpp
#pragma once

#include "../common/types.hpp"
#include <vector>


namespace nyas
{
    /// Take multi samples in single pixel
    class Sampler
    {
    public:
        /* Constructors */
        Sampler()
            : Sampler(1)
        {}
        Sampler(length_t n)
            : _num_samples(n)
            , _samples(n)
            , _count(0)
        {}
        Sampler(Sampler const&) = default;

        Sampler constexpr & operator=(Sampler const&) = default;

        /* Destructor */
        virtual ~Sampler() = default;


        /// Generate samples in range [0, 1]^2
        void virtual generate_samples() = 0;

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
        length_t _num_samples;
        std::vector<Point2D> _samples;
        length_t _count;
    };

} // namespace nyas

