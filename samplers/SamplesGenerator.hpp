/// @file samplers/Sampler.hpp
#pragma once

#include "../common/types.hpp"
#include <vector>


namespace nyas
{
    typedef ::std::vector<Point2D> SampleList;


    /// samples generator
    class SamplesGenerator
    {
    public:
        explicit SamplesGenerator(length_t const& num_sets)
            : _num_sets(num_sets)
            , _num_samples(1)
        {}
        explicit SamplesGenerator(length_t const& num_sets, length_t const& num_samples)
            : _num_sets(num_sets)
            , _num_samples(num_samples)
        {}

        virtual ~SamplesGenerator() = default;

        /// note that num_samples may not same as input.
        length_t inline num_samples() const
        {
            return this->_num_samples;
        }
        length_t inline num_sets() const
        {
            return this->_num_sets;
        }

        /// generate samples in range [0, 1]^2
        SampleList virtual generate_samples() const = 0;


    protected:
        length_t _num_sets;
        length_t _num_samples;
    };

} // namespace nyas

