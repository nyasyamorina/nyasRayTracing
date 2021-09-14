/// @file samplers/PureRandom.hpp
#pragma once

#include "SamplesGenerator.hpp"
#include "../common/randoms.hpp"


namespace nyas
{
    namespace samples_generators
    {
        /// pure random samples generator
        class PureRandom final : public SamplesGenerator
        {
        public:
            explicit PureRandom(length_t const& num_sets, length_t const& num_samples)
                : SamplesGenerator(num_sets, num_samples)
            {}

            SampleList virtual generate_samples() const override
            {
                SampleList list;
                list.reserve(this->_num_sets * this->_num_samples);
                for (length_t s = 0; s < this->_num_sets; ++s) {
                    for(length_t n = 0; n < this->_num_samples; ++n) {
                        list.push_back(random::uniform2D());
                    }
                }
                return list;
            }
        };

    } // namespace samples_generators

} // namespace nyas
