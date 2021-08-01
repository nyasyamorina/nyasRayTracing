#include "../../include/common/fast_inversesqrt.h"

#include <bit>
#include "../../include/common/functions.h"


namespace nyas
{
    // user should not use '_detail' namespace
    namespace _detail
    {
        template<typename genType>
        struct fast_inversesqrt
        {
            genType static inline call(genType x)
            {
                return static_cast<genType>(1) / sqrt(x);
            }
        };

        template <>
        struct fast_inversesqrt<float32>
        {
            float32 static inline constexpr call(float32 x)
            {
                float32 const halfx = x * 0.5f;
                float32 y = std::bit_cast<float32>(0x5f3759df - (std::bit_cast<uint32>(x) >> 1));
                // use once Newton's method for fast but higher error (+- 0.0018 Relative error)
                // y = y * (1.5f - (halfx * y * y));
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
                // use once Newton's method for fast but higher error (+- 0.0018 Relative error)
                // y = y * (1.5 - (halfx * y * y));
                // y = y * (1.5 - (halfx * y * y));
                // y = y * (1.5 - (halfx * y * y));
	            return y * (1.5 - (halfx * y * y));
            }
        };


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
    /// @tparam genType floating-point types (float or double)
    template<typename genType>
    genType inversesqrt(genType x)
    {
        static_assert(is_float<genType>(), "'inversesqrt' accepts only floating-point inputs");

        return _detail::fast_inversesqrt<genType>::call(x);
    }

    /// Return inverse square root of input, i.e., 1/sqrt(x)
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam T type of vector data, must be floating-point
    template<length_t L, typename T, glm::qualifier Q>
    vec<L, T, Q> inversesqrt(vec<L, T, Q> const& v)
    {
        static_assert(is_float<T>(), "'inversesqrt' accepts only floating-point inputs");

        return _detail::fast_inversesqrt_vec<L, T, Q>::call(v);
    }
}
