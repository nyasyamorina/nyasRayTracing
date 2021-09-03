/// @file samplers/Jittered.hpp
#pragma once

#include "Sampler.hpp"
#include "../common/randoms.hpp"


namespace nyas
{
    namespace samplers
    {
        class MultiJittered final : public Sampler
        {
        public:
            MultiJittered()
                : MultiJittered(1)
            {}
            MultiJittered(length_t side)
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
                float64 const subcell_size = 1. / static_cast<float64>(this->_num_samples);
                for (length_t y = 0; y < this->_side_length; ++y) {
                    for (length_t x = 0; x < this->_side_length; ++x) {
                        this->_samples.push_back(Point2D(x, y) * cell_size + (Point2D(y, x) + random::uniform2D()) * subcell_size);
                    }
                }
                this->_shuffle_x_coordinates();
                this->_shuffle_y_coordinates();
            }

            void _shuffle_x_coordinates()
            {
                for (length_t x = 0; x < this->_side_length; ++x) {
                    for (length_t y = 1; y < this->_side_length; ++y) {
                        std::swap(
                            this->_samples[(random::integer() % this->_side_length) * this->_side_length + x].x,
                            this->_samples[y * this->_side_length + x].x
                        );
                    }
                }
            }

            void _shuffle_y_coordinates()
            {
                for (length_t y = 0; y < this->_side_length; ++y) {
                    length_t line_start = y * this->_side_length;
                    for (length_t x = 1; x < this->_side_length; ++x) {
                        std::swap(
                            this->_samples[random::integer() % this->_side_length + line_start].y,
                            this->_samples[x + line_start].y
                        );
                    }
                }
            }
        };

    } // namespace sampler

} // namespace nyas
