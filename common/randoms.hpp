/// @file common/randoms.hpp
#pragma once

//#define RANDOM_WITH_TIME

#include "types.hpp"
#include <random>
#include <limits>
#ifdef RANDOM_WITH_TIME
    #include <chrono>
#endif


namespace nyas
{
    namespace _detail   // ! user should not use namespace '_detail'
    {
#ifdef RANDOM_WITH_TIME
        using namespace std::chrono;
#endif

        // ganerate global random generator seed
        std::minstd_rand0 static seed_generator(
#ifdef RANDOM_WITH_TIME
            static_cast<uint32>(system_clock::to_time_t(time_point_cast<milliseconds>(system_clock::now())))
#else
            1
#endif
        );

        // random generator in each thread
        std::mt19937 static thread_local random_generator(seed_generator());

        std::uniform_real_distribution static thread_local distribution01;

    } // namespace _detail

    namespace random
    {
        length_t inline integer()
        {
            // ! cannot return negative number
            return static_cast<length_t>(_detail::random_generator() & INT_MAX);
        }

        float64 inline uniform()
        {
            return _detail::distribution01(_detail::random_generator);
        }

        float64 inline uniform(float64 min, float64 max)
        {
            return (max - min) * uniform() + min;
        }

        vec<2, float64> inline uniform2D()
        {
            return vec<2, float64>(uniform(), uniform());
        }

        vec<3, float64> inline uniform3D()
        {
            return vec<3, float64>(uniform(), uniform(), uniform());
        }

    } // namespace random

} // namespace nyas

