/// @file samplers/Jittered.hpp
#pragma once

#include "SamplesGenerator.hpp"
#include "../common/functions.hpp"
#include "../common/randoms.hpp"
#include <algorithm>


namespace nyas
{
    namespace samples_generators
    {
        /// multi-jittered samples generator
        ///
        /// @param num_samples should be squared number
        class MultiJittered final : public SamplesGenerator
        {
        public:
            explicit MultiJittered(length_t const& num_sets, length_t const& num_samples)
                : SamplesGenerator(num_sets)
            {
                this->_num_side = length_t(sqrt(float64(num_samples)));
                this->_num_samples = this->_num_side * this->_num_side;
            }

            SampleList virtual generate_samples() const override
            {
                float64 const cell_size = 1. / this->_num_side;
                float64 const subcell_size = 1. / this->_num_samples;
                SampleList list;
                list.reserve(this->_num_sets * this->_num_samples);
                for (length_t s = 0; s < this->_num_sets; ++s) {
                    // generate samples in one set
                    for (length_t y = 0; y < this->_num_side; ++y) {
                        for (length_t x = 0; x < this->_num_side; ++x) {
                            list.push_back(Point2D(x, y) * cell_size + (Point2D(y, x) + random::uniform2D()) * subcell_size);
                        }
                    }
                    length_t set_start = s * this->_num_samples;
                    // shuffle x in one set
                    for (length_t y = 0; y < this->_num_side; ++y) {
                        for (length_t x = 0; x < this->_num_side; ++x) {
                            length_t k = y + random::integer() % (this->_num_side - y);
                            ::std::swap(
                                list[set_start + y * this->_num_side + x].x,
                                list[set_start + k * this->_num_side + x].x
                            );
                        }
                    }
                    // shuffle y in one set
                    for (length_t x = 0; x < this->_num_side; ++x) {
                        for (length_t y = 0; y < this->_num_side; ++y) {
                            length_t k = x + random::integer() % (this->_num_side - x);
                            ::std::swap(
                                list[set_start + y * this->_num_side + x].x,
                                list[set_start + y * this->_num_side + k].x
                            );
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
