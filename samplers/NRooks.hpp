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
                float64 const cell_size = 1. / static_cast<float64>(_side_length);
                for (length_t y = 0; y < _side_length; ++y) {
                    for (length_t x = 0; x < _side_length; ++x) {
                        _samples.push_back((Point2D(x, x) + random::uniform2D()) * cell_size);
                    }
                }
                _shuffle_coordinates();
            }

            void _shuffle_coordinates()
            {
                for (length_t y = 0; y < _side_length; ++y) {
                    length_t line_start = y * _side_length;
                    for (length_t x = 1; x < _side_length; ++x) {
                        std::swap(
                            _samples[(random::integer() % _side_length) + line_start].x,
                            _samples[x + line_start].x
                        );
                        std::swap(
                            _samples[(random::integer() % _side_length) + line_start].x,
                            _samples[x + line_start].x
                        );
                    }
                }
            }
        };

    } // namespace sampler

} // namespace nyas
