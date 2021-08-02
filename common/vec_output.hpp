/// @file common/vec_output.hpp
#pragma once

#include "types.hpp"
#include <iostream>

namespace nyas
{
    namespace _detail   // ! user should not use namespace '_detail'
    {
        char const* const commaspace = ", ";

        template<length_t L, typename T, glm::qualifier Q>
        struct vec_output;

        template<typename T, glm::qualifier Q>
        struct vec_output<1, T, Q>
        {
            ::std::ostream static inline & call(::std::ostream & out, vec<1, T, Q> const& v)
            {
                return out << '[' << v.x << ']';
            }
        };

        template<typename T, glm::qualifier Q>
        struct vec_output<2, T, Q>
        {
            ::std::ostream static inline & call(::std::ostream & out, vec<2, T, Q> const& v)
            {
                return out << '[' << v.x << commaspace << v.y << ']';
            }
        };

        template<typename T, glm::qualifier Q>
        struct vec_output<3, T, Q>
        {
            ::std::ostream static inline & call(::std::ostream & out, vec<3, T, Q> const& v)
            {
                return out << '[' << v.x << commaspace << v.y << commaspace << v.z << ']';
            }
        };

        template<typename T, glm::qualifier Q>
        struct vec_output<4, T, Q>
        {
            ::std::ostream static inline & call(::std::ostream & out, vec<4, T, Q> const& v)
            {
                return out << '[' << v.x << commaspace << v.y << commaspace << v.z << commaspace << v.w << ']';
            }
        };

    } // namespace _detail
} // namespace nyas


template<nyas::length_t L, typename T, glm::qualifier Q>
std::ostream inline & operator<<(std::ostream & out, nyas::vec<L, T, Q> const& v)
{
    return nyas::_detail::vec_output<L, T, Q>::call(out, v);
}
