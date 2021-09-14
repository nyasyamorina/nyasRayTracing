/// @file samplers/Jittered.hpp
#pragma once

#include "SamplesGenerator.hpp"
#include "../common/randoms.hpp"
#include <algorithm>


namespace nyas
{
    namespace samples_generators
    {
        /// n-Rooks samples generator
        class NRooks final : public SamplesGenerator
        {
        public:
            explicit NRooks(length_t const& num_sets, length_t const& num_samples)
                : SamplesGenerator(num_sets, num_samples)
            {}

            SampleList virtual generate_samples() const override
            {
                float64 const cell_size = 1. / this->_num_samples;
                SampleList list;
                list.reserve(this->_num_sets * this->_num_samples);
                for (length_t s = 0; s < this->_num_sets; ++s) {
                    // generate samples in one set
                    for(length_t n = 0; n < this->_num_samples; ++n) {
                        list.push_back((Point2D(n, n) + random::uniform2D()) * cell_size);
                    }
                    length_t set_start = s * this->_num_samples;
                    // shuffle in one set
                    for(length_t n = 0; n < this->_num_samples; ++n) {
                        length_t k = n + random::integer() % (this->_num_samples - n);
                        ::std::swap(
                            list[set_start + n].x,
                            list[set_start + k].x
                        );
                        k = n + random::integer() % (this->_num_samples - n);
                        ::std::swap(
                            list[set_start + n].y,
                            list[set_start + k].y
                        );
                    }
                }
                return list;
            }
        };

    } // namespace samples_generators

} // namespace nyas
