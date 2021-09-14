/// @file nyasRayTracing.hpp
#pragma once

// glm
////#include "glm/glm.hpp"

// setup
#include "common/setup.h"

// common
#include "common/types.hpp"
#include "common/constants.hpp"
#include "common/randoms.hpp"
#include "common/functions.hpp"

// utils
#include "utils.hpp"

// buffer 2D
#include "Buffer2D.hpp"

// sampler
#include "samplers/SamplesGenerator.hpp"
#include "samplers/Regular.hpp"
#include "samplers/PureRandom.hpp"
#include "samplers/Jittered.hpp"
#include "samplers/NRooks.hpp"
#include "samplers/MultiJittered.hpp"
#include "samplers/Hammersley.hpp"
#include "samplers/Sampler.hpp"

// ray
#include "Ray.hpp"

// camera
#include "cameras/Camera.hpp"
#include "cameras/Parallel.hpp"
#include "cameras/Pinhole.hpp"

// sky
#include "skies/Sky.hpp"
#include "skies/NoSky.hpp"
#include "skies/Zenith.hpp"

// brdf
#include "brdfs/BRDF.hpp"
#include "brdfs/Lambertian.hpp"

// object 3D
#include "objects/Object3D.hpp"
//#include "objects/MultiObject3D.hpp"
#include "objects/Sphere.hpp"

// ray tracer
#include "tracers/RayTracer.hpp"
#include "tracers/HemisphereModel.hpp"

// world
#include "World.hpp"
