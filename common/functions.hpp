/// @file common/funcions.hpp
#pragma once

#include "glm/glm.hpp"
#include "types.hpp"
#include "fast_inversesqrt.hpp"
#include <limits>


namespace nyas
{
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
    bool constexpr is_number()
    {
        return is_float<T>() || is_integer<T>();
    }


    using ::glm::sqrt;
    using ::glm::pow;
    using ::glm::clamp;

    using ::glm::dot;
    using ::glm::cross;
    using ::glm::length;
    using ::glm::distance;

    /// Return squared vector length, i.e., dot(vector, vector)
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam T type of vector data, must be floating-point
    template<length_t L, typename T, glm::qualifier Q>
    T length2(vec<L, T, Q> const& v)
    {
        return dot(v, v);
    }


#ifdef FAST_INVERSE_SQUARE_ROOT

    namespace _detail   // ! user should not use namespace '_detail'
    {
        template<length_t L, typename T, glm::qualifier Q>
        struct fast_inversesqrt_vec;

        template<typename T, glm::qualifier Q>
        struct fast_inversesqrt_vec<1, T, Q>
        {
            vec<1, T, Q> static inline call(vec<1, T, Q> const& v)
            {
                return vec<1, T, Q>(inversesqrt(v.x));
            }
        };

        template<typename T, glm::qualifier Q>
        struct fast_inversesqrt_vec<2, T, Q>
        {
            vec<2, T, Q> static inline call(vec<2, T, Q> const& v)
            {
                return vec<2, T, Q>(inversesqrt(v.x), inversesqrt(v.y));
            }
        };

        template<typename T, glm::qualifier Q>
        struct fast_inversesqrt_vec<3, T, Q>
        {
            vec<3, T, Q> static inline call(vec<3, T, Q> const& v)
            {
                return vec<3, T, Q>(inversesqrt(v.x), inversesqrt(v.y), inversesqrt(v.z));
            }
        };

        template<typename T, glm::qualifier Q>
        struct fast_inversesqrt_vec<4, T, Q>
        {
            vec<4, T, Q> static inline call(vec<4, T, Q> const& v)
            {
                return vec<4, T, Q>(inversesqrt(v.x), inversesqrt(v.y), inversesqrt(v.z), inversesqrt(v.w));
            }
        };

    } // namespace _detail

    /// Return inverse square root of input, i.e., 1/sqrt(x)
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam T type of vector data, must be floating-point
    /// @param v inversesqrt function is defined for input values of v defined in the range [0, inf+)
    template<length_t L, typename T, glm::qualifier Q>
    vec<L, T, Q> inversesqrt(vec<L, T, Q> const& v)
    {
        static_assert(is_float<T>(), "'inversesqrt' accepts only floating-point inputs");
        return _detail::fast_inversesqrt_vec<L, T, Q>::call(v);
    }

    /// Returns a vector in the same direction as x but with length of 1.
	/// According to issue 10 GLSL 1.10 specification, if length(x) == 0 then result is undefined and generate an error.
	///
	/// @tparam L An integer between 1 and 4 included that qualify the dimension of the vector.
	/// @tparam T Floating-point scalar types.
	template<length_t L, typename T, glm::qualifier Q>
    vec<L, T, Q> inline normalize(vec<L, T, Q> const& x)
    {
        static_assert(is_float<T>(), "'normalize' accepts only floating-point inputs");
        return x * inversesqrt(length2(x));
    }

#else

    using ::glm::inversesqrt;
    using ::glm::normalize;

#endif

} // namespace nyas
