//
// primitive.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <vector>
#include "ray.h"
#include "isect.h"
#include "material.h"
#include "primitive.h"

/// ---------------------------------------------------------------------------
/// @brief Primitive factory function with sphere geometry.
///
Primitive Primitive::Create(
    const math::vec3d &centre,
    const double radius,
    const Material &material)
{
    return {centre, radius, material};
}

/// ---------------------------------------------------------------------------
/// @brief Compute primitive-ray intersection.
///
bool Primitive::Intersect(
    const Primitive &primitive,
    const Ray &ray,
    const double t_min,
    const double t_max,
    double &t,
    math::vec3d &n)
{
    //
    // Solve the ray-sphere intersection problem:
    //      (p(t) - centre) * (p(t) - centre) = radius^2
    // where p(t) = o + t*d.
    //
    math::vec3d oc = ray.o - primitive.centre;
    double a = math::dot(ray.d, ray.d);
    double b = math::dot(ray.d, oc);
    double c = math::dot(oc, oc) - primitive.radius * primitive.radius;

    double discriminant = b*b - a*c;
    if (discriminant < 0.0) {
        return false;
    }
    discriminant = std::sqrt(discriminant);

    // Compute the line parameter and normal at the intersection point.
    t = -(b + discriminant) / a;
    if (t < t_min) {
        t = -(b - discriminant) / a;
    }
    if (t < t_min || t > t_max) {
        return false;   // line parameter outside intersection range
    }
    n = math::normalize(ray.at(t) - primitive.centre);
    return true;
}

///
/// @brief Compute primitive-ray intersection and store geometric properties.
///
bool Primitive::Intersect(
    const Primitive &primitive,
    const Ray &ray,
    const double t_min,
    const double t_max,
    Isect &isect)
{
    double t;
    math::vec3d n;
    if (Primitive::Intersect(primitive, ray, t_min, t_max, t, n)) {
        isect.p = ray.at(t);
        isect.n = n;
        isect.wo = -ray.d;
        isect.t = t;
        isect.material = primitive.material;
        return true;
    }
    return false;
}

///
/// @brief Compute the closest primitive-ray intersection.
///
bool Primitive::Intersect(
    const std::vector<Primitive> &primitives,
    const Ray &ray,
    const double t_min,
    const double t_max,
    Isect &isect)
{
    bool is_a_hit = false;
    double t_hit = t_max;
    for (const auto &primitive : primitives) {
        if (Primitive::Intersect(primitive, ray, t_min, t_hit, isect)) {
            is_a_hit = true;
            t_hit = isect.t;
        }
    }
    return is_a_hit;
}

/// ---------------------------------------------------------------------------
/// @brief Generate a random collection of primitives.
///
std::vector<Primitive> Primitive::Generate(int32_t n_cells)
{
    math::random_engine rng = math::make_random();
    math::random_uniform<float> dist;

    std::vector<Primitive> world;
    Material ground_material = Material::CreateDiffuse(Color{0.5, 0.5, 0.5});
    world.push_back(Primitive::Create(
        math::vec3d{0.0, -1000.0 , 0.0},
        1000,
        ground_material));

    for (int a = -n_cells; a < n_cells; a++) {
        for (int b = -n_cells; b < n_cells; b++) {
            math::vec3d centre{a + 0.9*dist(rng), 0.2, b + 0.9*dist(rng)};
            math::vec3d point{4.0, 0.2, 0.0};

            double choose_mat = dist(rng);
            if (math::norm(centre - point) > 0.9) {
                if (choose_mat < 0.8) {
                    // Diffuse sphere
                    Color rho = Color{dist(rng), dist(rng), dist(rng)} *
                                Color{dist(rng), dist(rng), dist(rng)};
                    Material sphere_material = Material::CreateDiffuse(rho);
                    world.push_back(Primitive::Create(
                        centre, 0.2, sphere_material));

                } else if (choose_mat < 0.95) {
                    // Conductor sphere
                    Color rho = Color{0.5, 0.5, 0.5};
                    rho += 0.5 * Color{dist(rng), dist(rng), dist(rng)};
                    Material sphere_material = Material::CreateConductor(rho);
                    world.push_back(Primitive::Create(
                        centre, 0.2, sphere_material));

                } else {
                    // Glass Sphere
                    Material sphere_material = Material::CreateDielectric(1.5);
                    world.push_back(Primitive::Create(
                        centre, 0.2, sphere_material));
                }
            }
        }
    }

    Material material1 = Material::CreateDielectric(1.5);
    world.push_back(Primitive::Create(math::vec3d{0, 1, 0}, 1.0, material1));

    Material material2 = Material::CreateDiffuse(Color{0.4, 0.2, 0.1});
    world.push_back(Primitive::Create(math::vec3d{-4, 1, 0}, 1.0, material2));

    Material material3 = Material::CreateConductor(Color{0.7, 0.6, 0.5});
    world.push_back(Primitive::Create(math::vec3d{4, 1, 0}, 1.0, material3));

    return world;
}
