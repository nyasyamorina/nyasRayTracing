/// @file samplers/Regular.hpp
#pragma once

#include "Sampler.hpp"


namespace nyas
{
    namespace samplers
    {
        class Regular : public Sampler
        {
        public:
            Regular()
                : Regular(1)
            {}

            Regular(length_t side)
                : Sampler(side)
            {
                _generate_samples();
            }


        protected:
            void virtual _generate_samples() override
            {
                float64 const _float0 = 1. / static_cast<float64>(_side_length);
                for (length_t y = 0; y < _side_length; ++y) {
                    for (length_t x = 0; x < _side_length; ++x) {
                        _samples.push_back((Point2D(x, y) + 0.5) * _float0);
                    }
                }
            }
        };

    } // namespace samples

} // namespace nyas
