/// @file include/common/fast_inversesqrt.h
/// @see https://en.wikipedia.org/wiki/Fast_inverse_square_root
#pragma once

#include "glm/glm.hpp"
#include "types.h"

#define FAST_INVERSE_SQUARE_ROOT


namespace nyas
{
    /// Return inverse square root of input, i.e., 1/sqrt(x)
    ///
    /// @tparam genType floating-point types (float or double)
    template<typename genType>
    genType inversesqrt(genType);

    /// Return inverse square root of input, i.e., 1/sqrt(x)
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam T type of vector data, must be floating-point
    template<length_t L, typename T, glm::qualifier Q>
    vec<L, T, Q> inversesqrt(vec<L, T, Q> const&);

} // namespace nyas


#include "../../inline/common/fast_inversesqrt.inl"
