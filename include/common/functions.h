/// @file include/common/functions.h
#pragma once

#include "glm/glm.hpp"
#include "types.h"
#include "fast_inversesqrt.h"


namespace nyas
{
    /// is 'T' a floating-point type? (float/double/long double)
    template<typename T>
    bool constexpr is_float();

    /// is 'T' a integer type? (bool/short/int/long/long long)
    template<typename T>
    bool constexpr is_integer();

    /// is 'T' a number type? (floting-point or integer)
    template<typename T>
    bool constexpr is_number();


    using ::glm::dot;
    using ::glm::cross;
    using ::glm::length;
    using ::glm::distance;

    /// Return squared vector length, i.e., dot(vector, vector)
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam T type of vector data, must be floating-point
    template<length_t L, typename T, glm::qualifier Q>
    T length2(vec<L, T, Q> const&);

#ifdef FAST_INVERSE_SQUARE_ROOT

    /// Returns a vector in the same direction as x but with length of 1.
	/// According to issue 10 GLSL 1.10 specification, if length(x) == 0 then result is undefined and generate an error.
	///
	/// @tparam L An integer between 1 and 4 included that qualify the dimension of the vector.
	/// @tparam T Floating-point scalar types.
	///
	/// @see <a href="http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml">GLSL normalize man page</a>
	/// @see <a href="http://www.opengl.org/registry/doc/GLSLangSpec.4.20.8.pdf">GLSL 4.20.8 specification, section 8.5 Geometric Functions</a>
	template<length_t L, typename T, glm::qualifier Q>
    vec<L, T, Q> normalize(vec<L, T, Q> const& x);

#else
    using ::glm::inversesqrt;
    using ::glm::normalize;
#endif

} // namespace nyas


#include "../../inline/common/functions.inl"
