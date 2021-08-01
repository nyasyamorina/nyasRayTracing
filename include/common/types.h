/// @file include/common/types.h
#pragma once

#include "glm/glm.hpp"


namespace nyas
{
    typedef float  float32;
    typedef double float64;

    typedef ::glm::length_t length_t;
    using ::glm::vec;

    typedef vec<3, float64> Point3D;
    typedef vec<3, float64> Vector3D;
    typedef vec<3, float32> RGBColor;

} // namespace nyas
