#include <iostream>

////#include "glm/glm.hpp"
#include "examples.hpp"
#include "cameras/Camera.hpp"
#include "cameras/Parallel.hpp"
#include "cameras/Pinhole.hpp"


int main() {
    nyas::test_inersesqrt();

    nyas::example_rendering_output();

    nyas::example_samplers();

    nyas::example_cameras();
}
