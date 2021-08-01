/// @file include/common/types.h
#pragma once

#include "glm/glm.hpp"


namespace nyas
{
    using ::glm::uint;
    using ::glm::uint8;
    using ::glm::uint16;
    using ::glm::uint32;
    using ::glm::uint64;

    using ::glm::int8;
    using ::glm::int16;
    using ::glm::int32;
    using ::glm::int64;

    typedef float  float32;
    typedef double float64;

    typedef ::glm::length_t length_t;
    using ::glm::vec;

    typedef vec<3, float64> Point3D;
    typedef vec<3, float64> Vector3D;
    typedef vec<3, float32> RGBColor;

} // namespace nyas
