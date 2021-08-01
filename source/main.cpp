#include <iostream>

#include "glm/glm.hpp"
#include "../include/common/types.h"
#include "../include/common/functions.h"
#include "../include/common/fast_inversesqrt.h"


int main() {
    // testing fast inversesqrt error
    nyas::vec<4, nyas::float64, glm::qualifier::defaultp> ccc(2., 3., 4., 5.);
    //ccc = nyas::inversesqrt(ccc);
    //ccc = nyas::normalize(ccc);
    //std::cout << ccc.x << ' ' << ccc.y << ' ' << ccc.z << ' ' << ccc.w << std::endl;
}
