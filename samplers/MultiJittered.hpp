/// @file samplers/Jittered.hpp
#pragma once

#include "Sampler.hpp"
#include "../common/randoms.hpp"

#include "../common/vec_output.hpp"


namespace nyas
{
    namespace samplers
    {
        class MultiJittered : public Sampler
        {
        public:
            MultiJittered()
                : MultiJittered(1)
            {}

            MultiJittered(length_t side)
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
                float64 const subcell_size = 1. / static_cast<float64>(_side_length * _side_length);
                for (length_t y = 0; y < _side_length; ++y) {
                    for (length_t x = 0; x < _side_length; ++x) {
                        Point2D target = Point2D(x, y) * cell_size + (Point2D(y, x) + random::uniform2D()) * subcell_size;
                        _samples.push_back(target);
                        std::cout << target << std::endl;
                    }
                }
                _shuffle_x_coordinates();
                _shuffle_y_coordinates();
            }

            void _shuffle_x_coordinates()
            {
                for (length_t x = 0; x < _side_length; ++x) {
                    for (length_t y = 1; y < _side_length; ++y) {
                        std::swap(
                            _samples[(random::integer() % _side_length) * _side_length + x].x,
                            _samples[y * _side_length + x].x
                        );
                    }
                }
            }

            void _shuffle_y_coordinates()
            {
                for (length_t y = 0; y < _side_length; ++y) {
                    length_t line_start = y * _side_length;
                    for (length_t x = 1; x < _side_length; ++x) {
                        std::swap(
                            _samples[random::integer() % _side_length + line_start].y,
                            _samples[x + line_start].y
                        );
                    }
                }
            }
        };

    } // namespace sampler

} // namespace nyas
