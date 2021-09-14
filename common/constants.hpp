/// @file common/constants.hpp
#pragma once

#include "types.hpp"
#include <limits>


namespace nyas
{
    template<typename FLOATx>
    struct constants final
    {
        static_assert(is_floating_point<FLOATx>::value, "'constants' accepts only floating-points");

        FLOATx constexpr static zero = static_cast<FLOATx>(0);                                                      // 0
        FLOATx constexpr static one = static_cast<FLOATx>(1);                                                       // 1
        FLOATx constexpr static two_pi = static_cast<FLOATx>(6.28318530717958647692528676655900576);                // 2 pi
        FLOATx constexpr static root_pi = static_cast<FLOATx>(1.772453850905516027);                                // sqrt(pi)
        FLOATx constexpr static pi_over_two = static_cast<FLOATx>(1.57079632679489661923132169163975144);           // pi / 2
        FLOATx constexpr static three_over_two_pi = static_cast<FLOATx>(4.71238898038468985769396507491925432);     // 1.5 / pi
        FLOATx constexpr static pi_over_four = static_cast<FLOATx>(0.785398163397448309615660845819875721);         // pi / 4
        FLOATx constexpr static one_over_pi = static_cast<FLOATx>(0.318309886183790671537767526745028724);          // 1 / pi
        FLOATx constexpr static one_over_two_pi = static_cast<FLOATx>(0.159154943091895335768883763372514362);      // 0.5 / pi
        FLOATx constexpr static two_over_pi = static_cast<FLOATx>(0.636619772367581343075535053490057448);          // 2 / pi
        FLOATx constexpr static four_over_pi = static_cast<FLOATx>(1.273239544735162686151070106980114898);         // 4 / pi
        FLOATx constexpr static two_over_root_pi = static_cast<FLOATx>(1.12837916709551257389615890312154517);      // 2 / sqrt(pi)
        FLOATx constexpr static one_over_root_two = static_cast<FLOATx>(0.707106781186547524400844362104849039);    // 1 / sqrt(2)
        FLOATx constexpr static root_half_pi = static_cast<FLOATx>(1.253314137315500251);                           // sqrt(pi / 2)
        FLOATx constexpr static root_two_pi = static_cast<FLOATx>(2.506628274631000502);                            // sqrt(2 pi)
        FLOATx constexpr static root_ln_four = static_cast<FLOATx>(1.17741002251547469);                            // sqrt(ln(4))
        FLOATx constexpr static e = static_cast<FLOATx>(2.71828182845904523536);                                    // e
        FLOATx constexpr static root_two = static_cast<FLOATx>(1.41421356237309504880168872420969808);              // sqrt(2)
        FLOATx constexpr static root_three = static_cast<FLOATx>(1.73205080756887729352744634150587236);            // sqrt(3)
        FLOATx constexpr static root_five = static_cast<FLOATx>(2.23606797749978969640917366873127623);             // sqrt(5)
        FLOATx constexpr static ln_two = static_cast<FLOATx>(0.693147180559945309417232121458176568);               // ln(2)
        FLOATx constexpr static ln_ten = static_cast<FLOATx>(2.30258509299404568401799145468436421);                // ln(10)
        FLOATx constexpr static ln_ln_two = static_cast<FLOATx>(-0.3665129205816643);                               // ln(ln(2))
        FLOATx constexpr static one_over_three = static_cast<FLOATx>(0.3333333333333333333333333333333333333333);   // 1 / 3
        FLOATx constexpr static two_over_three = static_cast<FLOATx>(0.666666666666666666666666666666666666667);    // 2 / 3
        FLOATx constexpr static golden_ratio = static_cast<FLOATx>(1.61803398874989484820458683436563811);          // phi
        FLOATx constexpr static pi = static_cast<FLOATx>(3.14159265358979323846264338327950288);                    // pi
        FLOATx constexpr static cos_one_over_two = static_cast<FLOATx>(0.877582561890372716130286068203503191);     // cos(0.5)
        //FLOATx constexpr static inverse_UINT_MAX = static_cast<FLOATx>(2.328306437080797375305252217003726400434970855712890625e-10); // 1 / ((2 ^ -32) - 1)
        FLOATx constexpr static inverse_two_to_32_power = static_cast<FLOATx>(2.3283064365386962890625e-10);        // 2 ^ -32
        FLOATx constexpr static euler = static_cast<FLOATx>(0.577215664901532860606);
        FLOATx constexpr static epsilon = ::std::numeric_limits<FLOATx>::epsilon();
        FLOATx constexpr static infinity = ::std::numeric_limits<FLOATx>::infinity();

        struct axis2D final
        {
            vec<2, FLOATx> constexpr static O = vec<2, FLOATx>(static_cast<FLOATx>(0));
            vec<2, FLOATx> constexpr static X = vec<2, FLOATx>(static_cast<FLOATx>(1), static_cast<FLOATx>(0));
            vec<2, FLOATx> constexpr static Y = vec<2, FLOATx>(static_cast<FLOATx>(0), static_cast<FLOATx>(1));
        };

        struct axis3D final
        {
            vec<3, FLOATx> constexpr static O = vec<3, FLOATx>(static_cast<FLOATx>(0));
            vec<3, FLOATx> constexpr static X = vec<3, FLOATx>(static_cast<FLOATx>(1), static_cast<FLOATx>(0), static_cast<FLOATx>(0));
            vec<3, FLOATx> constexpr static Y = vec<3, FLOATx>(static_cast<FLOATx>(0), static_cast<FLOATx>(1), static_cast<FLOATx>(0));
            vec<3, FLOATx> constexpr static Z = vec<3, FLOATx>(static_cast<FLOATx>(0), static_cast<FLOATx>(0), static_cast<FLOATx>(1));
        };
    };


} // namespace nyas
