/// @file samplers/Regular.hpp
#pragma once

#include "Sampler.hpp"


namespace nyas
{
    namespace samplers
    {
        class Regular final : public Sampler
        {
        public:
            Regular()
                : Regular(1)
            {}
            Regular(length_t side)
                : Sampler(side)
            {
                this->_generate_samples();
            }


        private:
            void virtual _generate_samples() override
            {
                float64 const cell_size = 1. / static_cast<float64>(this->_side_length);
                for (length_t y = 0; y < this->_side_length; ++y) {
                    for (length_t x = 0; x < this->_side_length; ++x) {
                        this->_samples.push_back((Point2D(x, y) + 0.5) * cell_size);
                    }
                }
            }
        };

    } // namespace samples

} // namespace nyas
