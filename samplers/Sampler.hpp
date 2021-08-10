/// @file samplers/Sampler.hpp
#pragma once

#include "../common/types.hpp"
#include <vector>


namespace nyas
{
    /// Take multi samples in single pixel.
    /// number of samples should be squares.
    ///
    /// @param n side length of sampler grid
    class Sampler
    {
    public:
        typedef std::vector<Point2D> SampleList;


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

