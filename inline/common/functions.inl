#include "../../include/common/functions.h"


namespace nyas
{

    /// Return squared vector length, i.e., dot(vector, vector)
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam T type of vector data, must be floating-point
    template<length_t L, typename T, glm::qualifier Q>
    T inline length2(vec<L, T, Q> const& v)
    {
        return dot(v, v);
    }

} // namespace nyas
