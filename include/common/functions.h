/// @file include/common/functions.h
#pragma once

#include "glm/glm.hpp"
#include "types.h"


namespace nyas
{
    using ::glm::dot;

    /// Return squared vector length, i.e., dot(vector, vector)
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam T type of vector data, must be floating-point
    template<length_t L, typename T, glm::qualifier Q>
    T length2(vec<L, T, Q> const&);

} // namespace nyas


#include "../../inline/common/functions.inl"
