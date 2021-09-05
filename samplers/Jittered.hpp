/// @file samplers/Jittered.hpp
#pragma once

#include "Sampler.hpp"
#include "../common/randoms.hpp"


namespace nyas
{
    namespace samplers
    {
        class Jittered final : public Sampler
        {
        public:
            Jittered()
                : Jittered(1)
            {}
            Jittered(length_t const& side)
                : Sampler(side)
            {
                if (this->_side_length == 1) {
                    this->_samples.push_back(Point2D(0.5));
                }
                else {
                    this->_generate_samples();
                }
            }


        private:
            void virtual _generate_samples() override
            {
                float64 const cell_size = 1. / static_cast<float64>(this->_side_length);
                for (length_t y = 0; y < this->_side_length; ++y) {
                    for (length_t x = 0; x < this->_side_length; ++x) {
                        this->_samples.push_back((Point2D(x, y) + random::uniform2D()) * cell_size);
                    }
                }
            }
        };

    } // namespace sampler

} // namespace nyas
