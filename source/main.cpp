#include <iostream>

//#include "glm/glm.hpp"
#include "../include/common/types.h"
#include "../include/common/functions.h"


int main() {
    std::cout << "Hello, World!" << std::endl;

    nyas::Point3D ccc(1.);

    double ctct = nyas::length2(ccc);

    std::cout << ccc.x << ' ' << ctct << std::endl;
}
