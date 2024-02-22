//
// isect.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef ISECT_H_
#define ISECT_H_

#include "common.h"
#include "color.h"
#include "ray.h"
#include "material.h"

///
/// @brief Hold the intersection properties between a ray and a shape - line
/// parameter, position, normal, etc.
///
struct Isect {
    double t;
    math::vec3d p;
    math::vec3d n;
    math::vec3d wo;
    Material material;

    // Return the absolute dot product |w.v|.
    static double AbsDot(const math::vec3d &v, const math::vec3d &w);

    // Are vectors wo and wi on the same hemisphere specified by normal n?
    static bool SameHemisphere(
        const math::vec3d &n,
        const math::vec3d &wo,
        math::vec3d &wi);

    // Orient w to lie on the same hemisphere specified by n.
    static math::vec3d FaceForward(
        const math::vec3d &n,
        const math::vec3d &w);

    // Return the direction vector from source towards the specified target.
    static math::vec3d DirTo(
        const math::vec3d &source,
        const math::vec3d &target);

    // Compute the incident vector wi that is reflected to wo.
    static void Reflect(
        const math::vec3d &n,
        const math::vec3d &wo,
        math::vec3d &wi);

    // Compute the incident vector wi that is refracted to wo.
    static bool Refract(
        const double eta,
        const math::vec3d &n,
        const math::vec3d &wo,
        math::vec3d &wi);

    // Return the reflectance of a conductor using Schlick approximation.
    static Color SchlickConductor(const Color &R0, const double cos_theta_i);

    // Return the reflectance of a dielectric using Schlick approximation.
    static double SchlickDielectric(const double eta, const double cos_theta_i);

    // Return the intersection indicident direction and scattering functions.
    static bool Scatter(
        const Isect &isect,
        const math::vec2d &u,
        const math::vec3d &wo,
        math::vec3d &wi,
        Color &bsdf,
        double &pdf);

    // Spawn a ray from the intersection point in the specified direction.
    static Ray Spawn(const Isect &isect, const math::vec3d &dir);

    // Spawn a ray from the intersection point towards the specified target.
    static Ray SpawnTo(const Isect &isect, const math::vec3d &target);
};

#endif // ISECT_H_
