/// @file common/types.hpp
#pragma once

#include "glm/glm.hpp"
#include <limits>


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
    template<length_t L, typename T> using vec = ::glm::vec<L, T, ::glm::qualifier::defaultp>;

    typedef vec<2, float64>  Point2D;
    typedef vec<3, float64>  Point3D;
    typedef vec<3, float64>  Vector3D;
    typedef vec<3, float32>  RGBColor;
    typedef vec<3, uint8>    DisplayRGBColor;
    typedef vec<2, length_t> Length2D;


    /// is 'T' a floating-point type? (float/double/long double)
    template<typename T>
    bool constexpr inline is_float()
    {
        return std::numeric_limits<T>::is_iec559;
    }

    /// is 'T' a integer type? (bool/short/int/long/long long)
    template<typename T>
    bool constexpr inline is_integer()
    {
        return std::numeric_limits<T>::is_integer;
    }

    /// is 'T' a number type? (floting-point or integer)
    template<typename T>
    bool constexpr inline is_number()
    {
        return is_float<T>() || is_integer<T>();
    }

} // namespace nyas
