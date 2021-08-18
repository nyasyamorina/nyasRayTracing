#include <iostream>

////#include "glm/glm.hpp"
#include "examples.hpp"
#include "samplers/Sampler.hpp"
#include "samplers/Regular.hpp"
#include "samplers/PureRandom.hpp"
#include "samplers/Jittered.hpp"


int main() {
    nyas::test_inersesqrt();

    nyas::example_rendering_output();

    nyas::example_samplers();
}
