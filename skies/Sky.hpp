/// @file skies/Sky.hpp
#pragma once

#include "../common/types.hpp"


namespace nyas
{
    class Sky
    {
    public:
        RGBColor virtual get_color(Vector3D const& direction) const = 0;
    };

    typedef shared_ptr<Sky> SkyPtr;
    typedef shared_ptr<Sky const> SkyConstptr;

} // namespace nyas
