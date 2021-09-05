/// @file common/funcions.hpp
#pragma once

#include "setup.h"
#include "glm/glm.hpp"
#include "types.hpp"
#include "constants.hpp"
#ifdef USE_FAST_SQUARE_ROOT
    #include "fast_inversesqrt.hpp"
#endif


namespace nyas
{
    /// literal suffix 'degree' for angle
    float128 constexpr inline operator""_deg(float128 degree)
    {
        constexpr float128 to_radian = pi<float128>() / 180.;
        return degree * to_radian;
    }

    /// literal suffix 'radian' for angle
    float128 constexpr inline operator""_rad(float128 radian)
    {
        return radian;
    }


    /* using math functions already in glm */

    using ::glm::sqrt;
    using ::glm::pow;
    using ::glm::clamp;
    using ::glm::abs;
    using ::glm::mod;
    using ::glm::sin;
    using ::glm::cos;
    using ::glm::tan;
    using ::glm::asin;
    using ::glm::acos;
    using ::glm::atan;

    using ::glm::dot;
    using ::glm::cross;
    using ::glm::length;
    using ::glm::distance;


    /* define new math functions */

    template<length_t L, typename T>
    vec<L, T> inline pow(vec<L, T> const& v, T const& exponent)
    {
        static_assert(is_floating_point<T>::value, "'pow' accepts only floating-point input");
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


    namespace _detail   // ! user should not use namespace '_detail'
    {
        template<length_t L, typename T>
        struct _delta
        {
            T static constexpr value = static_cast<T>(0);
        };

        template<>
        struct _delta<1, float32>
        {
            float32 static constexpr value = 1e-12f;
        };

        template<>
        struct _delta<2, float32>
        {
            float32 static constexpr value = 1e-6f;
        };

        template<>
        struct _delta<3, float32>
        {
            float32 static constexpr value = 1e-4f;
        };

        template<>
        struct _delta<4, float32>
        {
            float32 static constexpr value = 1e-3f;
        };

        template<>
        struct _delta<1, float64>
        {
            float64 static constexpr value = 1e-24;
        };

        template<>
        struct _delta<2, float64>
        {
            float64 static constexpr value = 1e-12;
        };

        template<>
        struct _delta<3, float64>
        {
            float64 static constexpr value = 1e-8;
        };

        template<>
        struct _delta<4, float64>
        {
            float64 static constexpr value = 1e-6;
        };

        template<length_t L, typename genType>
        bool constexpr inline _near_to_zero(genType x)
        {
            return -_detail::_delta<L, genType>::value <= x && x <= _detail::_delta<L, genType>::value;
        }


        template<length_t L, typename T>
        struct vec_near_to_zero;

        template<typename T>
        struct vec_near_to_zero<1, T>
        {
            bool static constexpr inline call(vec<1, T> const& v)
            {
                return _near_to_zero<1, T>(v.x);
            }
        };

        template<typename T>
        struct vec_near_to_zero<2, T>
        {
            bool static constexpr inline call(vec<2, T> const& v)
            {
                return _near_to_zero<2, T>(v.x) && _near_to_zero<2, T>(v.y);
            }
        };

        template<typename T>
        struct vec_near_to_zero<3, T>
        {
            bool static constexpr inline call(vec<3, T> const& v)
            {
                return _near_to_zero<3, T>(v.x) && _near_to_zero<3, T>(v.y) && _near_to_zero<3, T>(v.z);
            }
        };

        template<typename T>
        struct vec_near_to_zero<4, T>
        {
            bool static constexpr inline call(vec<4, T> const& v)
            {
                return _near_to_zero<4, T>(v.x) && _near_to_zero<4, T>(v.y) && _near_to_zero<4, T>(v.z) && _near_to_zero<4, T>(v.w);
            }
        };

    } // namespace _detail

    /// Return input is near to 0 or not.
    ///
    /// @tparam genType floating-point type
    template<typename genType>
    bool constexpr inline near_to_zero(genType const& x)
    {
        static_assert(is_floating_point<genType>::value, "'near_to_zero' only accepts floating-point input");
        return _detail::_near_to_zero<1, genType>(x);
    }

    /// Return input is near to 0 or not.
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam genType floating-point type
    template<length_t L, typename T>
    bool constexpr inline near_to_zero(vec<L, T> const& v)
    {
        return _detail::vec_near_to_zero<L, T>::call(v);
    }


    template<typename T>
    struct axis3D
    {
        vec<3, T> static constexpr inline O()
        {
            return vec<3, T>(static_cast<T>(0));
        }
        vec<3, T> static constexpr inline X()
        {
            return vec<3, T>(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
        }
        vec<3, T> static constexpr inline Y()
        {
            return vec<3, T>(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
        }
        vec<3, T> static constexpr inline Z()
        {
            return vec<3, T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
        }
    };

    template<typename T>
    struct axis2D
    {
        vec<2, T> static constexpr inline O()
        {
            return vec<2, T>(static_cast<T>(0));
        }
        vec<2, T> static constexpr inline X()
        {
            return vec<2, T>(static_cast<T>(1), static_cast<T>(0));
        }
        vec<2, T> static constexpr inline Y()
        {
            return vec<2, T>(static_cast<T>(0), static_cast<T>(1));
        }
    };


    /// Reduce number into range [0, 1], control by macro "OVERRANGE_REPEAT_MODE"
    ///
    /// @tparam L dimension of 'vec', must be in the 1 to 4 range
    /// @tparam genType floating-point type
    template<length_t L, typename T>
    vec<L, T> inline reduce_over01(vec<L, T> const& v)
    {
        static_assert(is_floating_point<T>::value, "'reduce_over01' accepts only floating-point input");
#if (OVERRANGE_REPEAT_MODE == 0)
        return clamp(v, 0., 1.);
#elif (OVERRANGE_REPEAT_MODE == 1)
        return ::glm::fract(v);
#elif (OVERRANGE_REPEAT_MODE == 2)
        return static_cast<T>(1) - abs(static_cast<T>(1) - mod(v, static_cast<T>(2)));
#else
#error unknown value on OVERRANGE_REPEAT_MODE
#endif
    }


#ifdef USE_FAST_SQUARE_ROOT

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
        static_assert(is_floating_point<T>::value, "'inversesqrt' accepts only floating-point inputs");
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
        static_assert(is_floating_point<T>::value, "'normalize' accepts only floating-point inputs");
        return x * inversesqrt(length2(x));
    }

#else

    using ::glm::inversesqrt;
    using ::glm::normalize;

#endif

} // namespace nyas
