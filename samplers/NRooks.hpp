/// @file samplers/Jittered.hpp
#pragma once

#include "Sampler.hpp"
#include "../common/randoms.hpp"
#include <algorithm>


namespace nyas
{
    namespace samplers
    {
        class NRooks final : public Sampler
        {
        public:
            NRooks()
                : NRooks(1)
            {}
            NRooks(length_t side)
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
                float64 cell_size = 1. / static_cast<float64>(this->_num_samples);
                for (length_t t = 0; t < this->_num_samples; ++t) {
                    this->_samples.push_back((Point2D(t, t) + random::uniform2D()) * cell_size);
                }
                this->_shuffle_coordinates();
            }

            void _shuffle_coordinates()
            {
                for (length_t t = 0; t < _num_samples; ++t) {
                    std::swap(
                        this->_samples[random::integer() % this->_num_samples].x,
                        this->_samples[t].x
                    );
                    std::swap(
                        this->_samples[random::integer() % this->_num_samples].y,
                        this->_samples[t].y
                    );
                }
            }
        };

    } // namespace sampler

} // namespace nyas
