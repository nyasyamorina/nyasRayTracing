/// @file skies/NoSky.hpp
#pragma once

#include "Sky.hpp"
#include "../common/types.hpp"


namespace nyas
{
    namespace skies
    {
        class NoSky final : public Sky
        {
        public:
            RGBColor constexpr static color = RGBColor(0.f);

            RGBColor virtual get_color(Vector3D const& direction) const override
            {
                return NoSky::color;
            }
        };

        typedef shared_ptr<NoSky> NoSkyPtr;
        typedef shared_ptr<NoSky const> NoSkyConstptr;

    } // namespace skies

} // namespace nyas
