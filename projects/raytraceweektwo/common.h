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
#include "core/graphics/graphics.h"

// Model parameters.
static const uint32_t kFilmWidth = 400;
static const uint32_t kFilmHeight = 300;
static const math::vec3d kCameraEye = {13.0, 2.0, 3.0};
static const math::vec3d kCameraCtr = { 0.0, 0.0, 0.0};
static const math::vec3d kCameraUp  = { 0.0, 1.0, 0.0};
static const double kCameraFocus = math::norm(kCameraCtr - kCameraEye);
static const double kCameraAperture = 0.25;
static const double kCameraFov = 20.0;
static const size_t kNumSamples = 128;
static const size_t kMaxSampleDepth = 64;

#endif // COMMON_H_
