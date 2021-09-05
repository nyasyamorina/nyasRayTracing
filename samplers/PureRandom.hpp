/// @file samplers/PureRandom
#pragma once

#include "Sampler.hpp"
#include "../common/randoms.hpp"


namespace nyas
{
    namespace samplers
    {
        class PureRandom final : public Sampler
        {
        public:
            PureRandom()
                : PureRandom(1)
            {}
            PureRandom(length_t const& side)
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
                for (length_t i = 0; i < this->_num_samples; ++i) {
                    this->_samples.push_back(random::uniform2D());
                }
            }
        };

    } // namespace samplers

} // namespace nyas
