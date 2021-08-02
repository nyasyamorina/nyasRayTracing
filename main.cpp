#include <iostream>

////#include "glm/glm.hpp"
#include "common/types.hpp"
#include "common/vec_output.hpp"
#include "common/functions.hpp"
#include "GraphicsBuffer.hpp"


nyas::RGBColor func1(nyas::length_t, nyas::length_t);


int main(int, char**) {
    /* testing fast inversesqrt error */
    /* output-fast: [0.70693, 0.576846, 0.499154, 0.447141] */
    /* output-default: [0.707107, 0.57735, 0.5, 0.447214] */
    nyas::vec<4, nyas::float64, glm::qualifier::defaultp> ccc(2., 3., 4., 5.);
    ccc = glm::inversesqrt(ccc);     // nyas::inversesqrt - fast; glm::inversesqrt - default
    ////ccc = nyas::normalize(ccc);
    std::cout << ccc << std::endl;

    /* testing graphics buffer and save to bmp */
    /* it will output an image as well as `./outputs/gradient_color.bmp` */
    nyas::length_t const WIDTH = 400;
    nyas::length_t const HEIGHT = 400;
    nyas::GraphicsBuffer<nyas::float32> gbuff(WIDTH, HEIGHT);   // rendering buffer
    nyas::GraphicsBuffer<nyas::uint8> ibuff(gbuff.size());      // image output buffer
    gbuff.for_each_index(
        [WIDTH, HEIGHT] (nyas::length_t w, nyas::length_t h, nyas::RGBColor & color)    // rendering method
        {
            color.r = float(w) / (WIDTH - 1);
            color.g = float(h) / (HEIGHT - 1);
            color.b = 0.2f;
        }
    );
    gbuff.for_each(
        [] (nyas::RGBColor & color)         // mapping color to display ouput
        {
            // pow(color, 1/2.2) is for gamma correction (https://en.wikipedia.org/wiki/Gamma_correction)
            color = nyas::clamp(256.f * nyas::pow(color, nyas::RGBColor(1.f / 2.2f)), 0.f, 255.f);
        }
    );
    gbuff.cast(ibuff);
    nyas::save_bmp("gradient_color.bmp", ibuff);
}
