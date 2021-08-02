cmake_minimum_required(VERSION 3.0.0)
project(nyasRayTracing)
set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_executable(nyasRayTracing main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


# glm path
# glm project: https://github.com/g-truc/glm
set(GLM_DIR ${PROJECT_SOURCE_DIR}/third-party/glm-0.9.9.8)


if (NOT EXISTS ${GLM_DIR})
    message("Please set your own `glm` path")
endif()
include_directories(${GLM_DIR})
