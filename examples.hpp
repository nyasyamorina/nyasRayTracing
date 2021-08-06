/// @file examples.hpp
#pragma once

#include "common/types.hpp"
#include "common/functions.hpp"
#include "common/vec_output.hpp"
#include "GraphicsBuffer.hpp"

#include <iostream>


namespace nyas
{
    /// test for fast inverse square root.
    void test_inersesqrt()
    {
        /* some number in a dvec4 */
        /* also use float32 for low precision floating-point */
        vec<4, float64> exam(2., 3., 4., 5.);
        auto default_res = glm::inversesqrt(exam);
        auto fast_res = nyas::inversesqrt(exam);        // fast inversesqrt
        ::std::cout << "default result: " << default_res << ::std::endl;
        ::std::cout << "fast result: " << fast_res << ::std::endl;
        ::std::cout << "relative error: " << ((fast_res - default_res) / exam) << ::std::endl << ::std::endl;
    }


    /// example for graphics buffer, rendering process and output an image
    void example_rendering_output()
    {
        /* set image size */
        length_t const width = 400;
        length_t const height = 400;
        /* alloc buffers */
        // gbuff uses float32 type to store color data, value should in range [0.f, 1.f]
        GraphicsBuffer<float32> gbuff(width, height);
        /* if buffer is not alloc correctly, then return */
        if (!gbuff.valid())
            return;
        /* use pixel index rendering color into gbuff */
        gbuff.for_each_index(
            [&width, &height] (length_t w, length_t h, RGBColor & color) {      // rendering method
                color.r = float(w) / (width - 1);
                color.g = float(h) / (height - 1);
                color.b = 0.2f;
            }
        );
        /* map floating-point color to display color in display output buffer */
        // ibuff is display output buffer, it uses uint8 type to store color data, value should in range [0, 255].
        GraphicsBuffer<uint8> ibuff = gbuff.map<uint8>(to_display);
        // save output bffer into bmp image
        save_bmp("gradient_color.bmp", ibuff);
    }

} // namespace nyas
