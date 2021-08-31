/// @file common/constants.hpp

#include "glm/gtc/constants.hpp"
#include "types.hpp"
#include <limits>


namespace nyas
{
    /*  glm is not include constant functions in `glm.hpp`
        so I manually include it and change namespace */
    using namespace ::glm;

    template<typename genType>
    genType constexpr inline infinity()
    {
        static_assert(is_floating_point<genType>::value, "'infinity' accepts only floating-point inputs");
        return ::std::numeric_limits<genType>::infinity();
    }

} // namespace nyas
