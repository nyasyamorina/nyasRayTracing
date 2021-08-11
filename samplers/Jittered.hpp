/// @file samplers/Jittered.hpp
#pragma once

#include "Sampler.hpp"
#include "../common/randoms.hpp"


namespace nyas
{
    namespace samplers
    {
        class Jittered : public Sampler
        {
        public:
            Jittered()
                : Jittered(1)
            {}

            Jittered(length_t side)
                : Sampler(side)
            {
                if (_side_length == 1) {
                    _samples.push_back(Point2D(0.5));
                }
                else {
                    _generate_samples();
                }
            }


        protected:
            void virtual _generate_samples() override
            {
                float64 const cell_size = 1. / static_cast<float64>(_side_length);
                for (length_t y = 0; y < _side_length; ++y) {
                    for (length_t x = 0; x < _side_length; ++x) {
                        _samples.push_back((Point2D(x, y) + random::uniform2D()) * cell_size);
                    }
                }
            }
        };

    } // namespace sampler

} // namespace nyas
