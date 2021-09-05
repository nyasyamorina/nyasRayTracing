/// @file common/setup.hpp
#pragma once


///////////////////////////////////////////////////////////////////////////////
/*                           setup some functions                            */
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// I switched from gcc to clang, however clang does not add macro `WIN32`
// if you are a linux user, please comment out this
#ifndef WIN32
#define WIN32
#endif
///////////////////////////////////////////////////////////////////////////////


// See code in 'common/fast_inversesqrt.hpp'. Require C++20 standard.
// Problem: it always throws an error: no templated named `bit_cast` in namespace 'std'.
//#define USE_FAST_SQUARE_ROOT

// if use this function, random methods will depends on system clock.
// otherwise, each run of program produces the same result
//#define RANDOM_WITH_TIME

// control how to ruduce numbers/points beyond range [0, 1]
// 0: clamp; 1: cycle roop; 2: mirror reflect; other: not define
#define OVERRANGE_REPEAT_MODE 2

// control reduce points beyond range on Buffer2D or not
#define REDUCE_POINT_BEYOND_RANGE
