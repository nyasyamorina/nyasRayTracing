/// @file samplers/Jittered.hpp
#pragma once

#include "Sampler.hpp"
#include "../common/randoms.hpp"
#include <algorithm>


namespace nyas
{
    namespace samplers
    {
        class NRooks : public Sampler
        {
        public:
            NRooks()
                : NRooks(1)
            {}

            NRooks(length_t side)
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
                float64 cell_size = 1. / static_cast<float64>(_num_samples);
                for (length_t t = 0; t < _num_samples; ++t) {
                    _samples.push_back((Point2D(t, t) + random::uniform2D()) * cell_size);
                }
                _shuffle_coordinates();
            }

            void _shuffle_coordinates()
            {
                for (length_t t = 0; t < _num_samples; ++t) {
                    std::swap(
                        _samples[random::integer() % _num_samples].x,
                        _samples[t].x
                    );
                    std::swap(
                        _samples[random::integer() % _num_samples].y,
                        _samples[t].y
                    );
                }
            }
        };

    } // namespace sampler

} // namespace nyas
