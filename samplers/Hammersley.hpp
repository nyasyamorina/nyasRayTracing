/// @file samplers/Hammersley.hpp
#pragma once

#include "Sampler.hpp"


namespace nyas
{
    namespace samplers
    {
        class Hammersley final : public Sampler
        {
        public:
            Hammersley()
                : Hammersley(1)
            {}
            Hammersley(length_t side)
                : Sampler(side)
            {
                if (this->_side_length == 1) {
                    this->_samples.push_back(Point2D(0.5));
                }
                else {
                    this->_generate_samples();
                }
            }


            float64 static constexpr phi(length_t x)
            {
                float64 y = 0., scaler = 0.5;
                while (x)
                {
                    y += scaler * (x & 1);
                    x >>= 1;
                    scaler *= 0.5;
                }
                return y;
            }


        private:
            void virtual _generate_samples() override
            {
                float64 const cell_size = 1. / static_cast<float64>(this->_num_samples);
                for (length_t t = 0; t < this->_num_samples; ++t) {
                    this->_samples.push_back(Point2D(t * cell_size, phi(t)));
                }
            }
        };

    } // namespace samples

} // namespace nyas
