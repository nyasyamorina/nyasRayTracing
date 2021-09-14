/// @file samplers/Regular.hpp
#pragma once

#include "SamplesGenerator.hpp"


namespace nyas
{
    namespace samples_generators
    {
        /// regular samples generator. it's num_sets is set to 1 because it is not random.
        ///
        /// @param num_samples should be squared number
        class Regular final : public SamplesGenerator
        {
        public:
            explicit Regular(length_t const& num_samples)
                : SamplesGenerator(1)
            {
                this->_num_side = length_t(sqrt(float64(num_samples)));
                this->_num_samples = this->_num_side * this->_num_side;
            }

            SampleList virtual generate_samples() const override
            {
                float64 const cell_size = 1. / this->_num_side;
                SampleList list;
                list.reserve(this->_num_samples);
                for (length_t y = 0; y < this->_num_side; ++y) {
                    for (length_t x = 0; x < this->_num_side; ++x) {
                        list.push_back((Point2D(x, y) + 0.5) * cell_size);
                    }
                }
                return list;
            }


        private:
            length_t _num_side;
        };

    } // namespace samples_generators

} // namespace nyas
