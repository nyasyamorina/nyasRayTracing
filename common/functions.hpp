/// @file common/funcions.hpp
#pragma once

#include "glm/glm.hpp"
#include "types.hpp"
#include "fast_inversesqrt.hpp"


namespace nyas
{
    /* using math functions already in glm */

    using ::glm::sqrt;
    using ::glm::pow;
    using ::glm::clamp;

    using ::glm::dot;
    using ::glm::cross;
    using ::glm::length;
    using ::glm::distance;


    /* define new math functions */

    template<length_t L, typename T>
    vec<L, T> inline pow(vec<L, T> const& v, T exponent)
    {
        static_assert(is_float<T>(), "'pow' accepts only floating-point input");
        return pow(v, vec<L, T>(exponent));
    }


    /// Return squared vector length, i.e., dot(vector, vector)
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam T type of vector data, must be floating-point
    template<length_t L, typename T>
    T inline length2(vec<L, T> const& v)
    {
        return dot(v, v);
    }


#ifdef FAST_INVERSE_SQUARE_ROOT

    namespace _detail   // ! user should not use namespace '_detail'
    {
        template<length_t L, typename T>
        struct fast_inversesqrt_vec;

        template<typename T>
        struct fast_inversesqrt_vec<1, T>
        {
            vec<1, T> static inline call(vec<1, T> const& v)
            {
                return vec<1, T>(inversesqrt(v.x));
            }
        };

        template<typename T>
        struct fast_inversesqrt_vec<2, T>
        {
            vec<2, T> static inline call(vec<2, T> const& v)
            {
                return vec<2, T>(inversesqrt(v.x), inversesqrt(v.y));
            }
        };

        template<typename T>
        struct fast_inversesqrt_vec<3, T>
        {
            vec<3, T> static inline call(vec<3, T> const& v)
            {
                return vec<3, T>(inversesqrt(v.x), inversesqrt(v.y), inversesqrt(v.z));
            }
        };

        template<typename T>
        struct fast_inversesqrt_vec<4, T>
        {
            vec<4, T> static inline call(vec<4, T> const& v)
            {
                return vec<4, T>(inversesqrt(v.x), inversesqrt(v.y), inversesqrt(v.z), inversesqrt(v.w));
            }
        };

    } // namespace _detail

    /// Return inverse square root of input, i.e., 1/sqrt(x)
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam T type of vector data, must be floating-point
    /// @param v inversesqrt function is defined for input values of v defined in the range [0, inf+)
    template<length_t L, typename T>
    vec<L, T> inline inversesqrt(vec<L, T> const& v)
    {
        static_assert(is_float<T>(), "'inversesqrt' accepts only floating-point inputs");
        return _detail::fast_inversesqrt_vec<L, T>::call(v);
    }

    /// Returns a vector in the same direction as x but with length of 1.
	/// According to issue 10 GLSL 1.10 specification, if length(x) == 0 then result is undefined and generate an error.
	///
	/// @tparam L An integer between 1 and 4 included that qualify the dimension of the vector.
	/// @tparam T Floating-point scalar types.
	template<length_t L, typename T>
    vec<L, T> inline normalize(vec<L, T> const& x)
    {
        static_assert(is_float<T>(), "'normalize' accepts only floating-point inputs");
        return x * inversesqrt(length2(x));
    }

#else

    using ::glm::inversesqrt;
    using ::glm::normalize;

#endif

} // namespace nyas
