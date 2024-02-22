//
// primitive.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <vector>
#include "common.h"
#include "ray.h"
#include "isect.h"
#include "material.h"

///
/// @brief A primitive is a geometric shape with a specified material.
/// @note Only spheres are considered.
/// @todo Extend shapes to disks, planes, meshes, etc.
///
struct Primitive {
    // Primitive geometry and material.
    math::vec3d centre;
    double radius;
    Material material;

    // Compute primitive-ray intersection.
    static bool Intersect(
        const Primitive &primitive,
        const Ray &ray,
        const double t_min,
        const double t_max,
        double &t,
        math::vec3d &n);

    // Compute primitive-ray intersection and store geometric properties.
    static bool Intersect(
        const Primitive &primitive,
        const Ray &ray,
        const double t_min,
        const double t_max,
        Isect &isect);

    // Compute the closest primitive-ray intersection.
    static bool Intersect(
        const std::vector<Primitive> &primitives,
        const Ray &ray,
        const double t_min,
        const double t_max,
        Isect &isect);

    // Primitive factory function with sphere geometry.
    static Primitive Create(
        const math::vec3d &centre,
        const double radius,
        const Material &material);

    // Generate a random collection of primitives.
    static std::vector<Primitive> Generate(int32_t n_cells);
};

#endif // PRIMITIVE_H_
