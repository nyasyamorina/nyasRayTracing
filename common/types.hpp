/// @file common/types.hpp
#pragma once

#include "glm/glm.hpp"
#include <type_traits>
#include <string>
#include <memory>


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

    typedef float       float32;
    typedef double      float64;
    typedef long double float128;

    typedef ::glm::length_t length_t;
    template<length_t L, typename T> using vec = ::glm::vec<L, T, ::glm::qualifier::defaultp>;

    typedef vec<2, float64>  Point2D;
    typedef vec<3, float64>  Point3D;
    typedef vec<3, float64>  Vector3D;
    typedef vec<3, float32>  RGBColor;
    typedef vec<3, uint8>    ImageRGBColor;
    typedef vec<2, length_t> Length2D;


    using ::std::is_floating_point;
    using ::std::is_integral;

    using ::std::string;

    using ::std::shared_ptr;
    using ::std::make_shared;

} // namespace nyas
