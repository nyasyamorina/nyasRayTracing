/// @file common/fast_inversesqrt.hpp
#pragma once


#define FAST_INVERSE_SQUARE_ROOT


#include "types.hpp"
#include <bit>


namespace nyas
{
    namespace _detail   // ! user should not use namespace '_detail'
    {
        template<typename genType>
        struct fast_inversesqrt
        {
            genType static inline call(genType x)
            {
                return static_cast<genType>(1) / ::glm::sqrt(x);
            }
        };

        template <>
        struct fast_inversesqrt<float32>
        {
            float32 static inline constexpr call(float32 x)
            {
                float32 const halfx = x * 0.5f;
                float32 y = std::bit_cast<float32>(0x5f3759df - (std::bit_cast<uint32>(x) >> 1));
                /* use once Newton's method for fast but higher error (+- 0.0018 Relative error) */
                y = y * (1.5f - (halfx * y * y));
	            return y * (1.5f - (halfx * y * y));
            }
        };

        template <>
        struct fast_inversesqrt<float64>
        {
            float64 static inline constexpr call(float64 x)
            {
                float64 const halfx = x * 0.5;
                float64 y = std::bit_cast<float64>(0x5fe6eb50c7b537a9 - (std::bit_cast<uint64>(x) >> 1));
                /* use once Newton's method for fast but higher error (+- 0.0018 Relative error) */
                //y = y * (1.5 - (halfx * y * y));
                //y = y * (1.5 - (halfx * y * y));
                //y = y * (1.5 - (halfx * y * y));
	            return y * (1.5 - (halfx * y * y));
            }
        };

    } // namespace _detail


    /// Return inverse square root of input, i.e., 1/sqrt(x)
    ///
    /// @tparam genType floating-point types (float or double)
    /// @param x inversesqrt function is defined for input values of x defined in the range [0, inf+)
    template<typename genType>
    genType inversesqrt(genType x)
    {
        static_assert(is_floating_point<genType>::value, "'inversesqrt' accepts only floating-point inputs");

        return _detail::fast_inversesqrt<genType>::call(x);
    }

} // namespace nyas

