/// @file samplers/Jittered.hpp
#pragma once

#include "SamplesGenerator.hpp"
#include "../common/functions.hpp"
#include "../common/randoms.hpp"


namespace nyas
{
    namespace samples_generators
    {
        /// jittered samples generator
        ///
        /// @param num_samples should be squared number
        class Jittered final : public SamplesGenerator
        {
        public:
            explicit Jittered(length_t const& num_sets, length_t const& num_samples)
                : SamplesGenerator(num_sets)
            {
                this->_num_side = length_t(sqrt(float64(num_samples)));
                this->_num_samples = this->_num_side * this->_num_side;
            }

            SampleList virtual generate_samples() const override
            {
                float64 const cell_size = 1. / this->_num_side;
                SampleList list;
                list.reserve(this->_num_sets * this->_num_samples);
                for (length_t s = 0; s < this->_num_sets; ++s) {
                    for (length_t y = 0; y < this->_num_side; ++y) {
                        for (length_t x = 0; x < this->_num_side; ++x) {
                            list.push_back((Point2D(x, y) + random::uniform2D()) * cell_size);
                        }
                    }
                }
                return list;
            }


        private:
            length_t _num_side;
        };

    } // namespace samples_generators

} // namespace nyas
