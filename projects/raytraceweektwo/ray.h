//
// ray.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef RAY_H_
#define RAY_H_

#include "common.h"

///
/// @brief Ray with origin at o and direction d, p(t) = o + t*d.
///
struct Ray {
    math::vec3d o;
    math::vec3d d;
    math::vec3d at(const double t) const { return (o + d*t); }
};

#endif // RAY_H_
