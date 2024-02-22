//
// sampler.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include "common.h"

///
/// @brief Sample a hemisphere, disk, cone, triangle, etc.
///
struct Sampler {
    // Sampler random number generator.
    math::random_engine m_engine;
    math::random_uniform<double> m_urand;

    // Sampler 1d and 2d uniform variates.
    double Rand1d();
    math::vec2d Rand2d();

    // Sampler a unit sphere using a uniform distribution.
    static math::vec3d UniformSphere(const math::vec2d &u);
    static double UniformSpherePdf();

    // Sampler a unit hemisphere using a uniform distribution.
    static math::vec3d UniformHemisphere(const math::vec2d &u);
    static double UniformHemispherePdf();

    // Sampler a unit hemisphere using a cosine distribution.
    static math::vec3d CosineHemisphere(const math::vec2d &u);
    static double CosineHemispherePdf(const double cos_theta);

    // Sampler a cone using a uniform distribution.
    static math::vec3d UniformCone(
        const math::vec2d &u,
        const double cos_theta_max);
    static double UniformConePdf(const double cos_theta_max);

    // Sampler unit disk using a uniform distribution.
    static math::vec2d UniformDisk(const math::vec2d &u);
    static double UniformDiskPdf();

    // Sampler unit triangle using a uniform distribution.
    static math::vec2d UniformTriangle(const math::vec2d &u);
    static double UniformTrianglePdf();

    // Sampler factory function.
    static Sampler Create(
        const math::random_engine &engine,
        const math::random_uniform<double> &urand);
};

#endif // SAMPLER_H_
