//
// common.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMMON_H_
#define COMMON_H_

#include "core/math/math.h"
#include "core/compute/compute.h"
#include "core/graphics/graphics.h"

// Raymarch parameters.
static const cl_uint kFilmWidth = 1024;         // camera film width
static const cl_uint kFilmHeight = 512;         // camera film height
static const cl_float kFilmDepth = 1.0;         // camera film depth
static const cl_float4 kSphereCentre = cl_float4{0.0f, 1.0f, -6.0f, 0.0f};
static const cl_float kSphereRadius = 1.0f;
static const cl_float kTmin = 0.01;             // raymarch min distance
static const cl_float kTmax = 1000.0;           // raymarch max distance
static const cl_uint kMaxSteps = 1000;          // raymarch max steps

// OpenCL parameters.
static const cl_ulong kDeviceIndex = 3;
static const cl_ulong kWorkGroupSize = 16;

#endif // COMMON_H_
