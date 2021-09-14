/// @file samplers/Hammersley.hpp
#pragma once

#include "SamplesGenerator.hpp"
#include "../common/constants.hpp"


namespace nyas
{
    namespace samples_generators
    {
        /// Hammersley samples generator. it's num_sets is set to 1 because it is not random.
        ///
        /// @param num_samples should be 2 of the integer power
        class Hammersley final : public SamplesGenerator
        {
        public:
            // The accuracy of this code has been verified, but it is inefficient
            ////float64 static constexpr phi(length_t const& x)
            ////{
            ////    float64 y = 0., scaler = 0.5;
            ////    while (x)
            ////    {
            ////        y += scaler * (x & 1);
            ////        x >>= 1;
            ////        scaler *= 0.5;
            ////    }
            ////    return y;
            ////}

            // ! This code is from Internet, feasibility to be verified
            float64 static constexpr phi(length_t const& x)
            {
                static_assert(sizeof(length_t) == sizeof(uint), "'Hammersley::phi' use bit operator to achieve.");
                uint bits = static_cast<uint>(x);   // ::std::bit_cast<uint>(x) in C++20 standard
                //reverse bit
                bits = (bits << 16) | (bits >> 16);
                bits = ((bits & 0x55555555) << 1) | ((bits & 0xAAAAAAAA) >> 1);
                bits = ((bits & 0x33333333) << 2) | ((bits & 0xCCCCCCCC) >> 2);
                bits = ((bits & 0x0F0F0F0F) << 4) | ((bits & 0xF0F0F0F0) >> 4);
                bits = ((bits & 0x00FF00FF) << 8) | ((bits & 0xFF00FF00) >> 8);
                return constants<float64>::inverse_two_to_32_power * bits;
            }


            explicit Hammersley(length_t const& num_samples)
                : SamplesGenerator(1)
            {
                assert(num_samples > 0);
                length_t num = num_samples;
                length_t log2_num = 0;
                if (num >> 16) {
                    num >>= 16;
                    log2_num += 16;
                }
                if (num >> 8) {
                    num >>= 8;
                    log2_num += 8;
                }
                if (num >> 4) {
                    num >>= 4;
                    log2_num += 4;
                }
                if (num >> 2) {
                    num >>= 2;
                    log2_num += 2;
                }
                if (num >> 1) {
                    num >>= 1;
                    log2_num += 1;
                }
                this->_num_samples = 1 << log2_num;
            }

            SampleList virtual generate_samples() const override
            {
                float64 const cell_size = 1. / this->_num_samples;
                SampleList list;
                list.reserve(this->_num_samples);
                for (length_t n = 0; n < this->_num_samples; ++n) {
                    list.push_back(Point2D(n * cell_size, Hammersley::phi(n)));
                }
                return list;
            }
        };

    } // namespace samples_generators

} // namespace nyas
