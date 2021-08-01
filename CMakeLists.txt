cmake_minimum_required(VERSION 3.18)
project(nyasRayTracing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)


set(GLM_DIR "./third-party/glm-0.9.9.8")       # set glm path
if (NOT EXISTS ${GLM_DIR})
    message("Please set your own 'glm' path")
endif()
include_directories(${GLM_DIR})


file(GLOB_RECURSE NYAS_INCLUDES "./include/*.h")
source_group("NYAS Files" FILES ${NYAS_INCLUDES})

#file(GLOB_RECURSE NYAS_INLINES "./inline/*.inl")
#source_group("NYAS Files" FILES ${NYAS_INLINES})

file(GLOB_RECURSE NYAS_SOURCES "./source/*.cpp")
source_group("NYAS Files" FILES ${NYAS_SOURCES})

add_executable(nyasRayTracing ${NYAS_SOURCES})
