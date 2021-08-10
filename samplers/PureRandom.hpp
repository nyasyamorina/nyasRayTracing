/// @file samplers/PureRandom
#pragma once

#include "Sampler.hpp"
#include "../common/randoms.hpp"


namespace nyas
{
    namespace samplers
    {
        class PureRandom : public Sampler
        {
        public:
            PureRandom()
                : PureRandom(1)
            {}

            PureRandom(length_t side)
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
                for (length_t i = 0; i < _num_samples; ++i) {
                    _samples.push_back(random::uniform2D());
                }
            }
        };

    } // namespace samplers

} // namespace nyas
