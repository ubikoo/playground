//
// sampler.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "common.h"
#include "sampler.h"

///
/// @brief Create a sampler object with the specified random number generator.
///
Sampler Sampler::Create(
    const math::random_engine &engine,
    const math::random_uniform<double> &urand)
{
    return {engine, urand};
}

///
/// @brief Rand1d 1-dimensional uniform variate.
///
double Sampler::Rand1d()
{
    return m_urand(m_engine, 0.0, 1.0);
}

///
/// @brief Sample 2-dimensional uniform variate.
///
math::vec2d Sampler::Rand2d()
{
    return {m_urand(m_engine, 0.0, 1.0), m_urand(m_engine, 0.0, 1.0)};
}

///
/// @brief Sample a unit sphere using a uniform distribution.
///
math::vec3d Sampler::UniformSphere(const math::vec2d &u)
{
    double cos_theta = 1.0 - 2.0 * u.x;
    double sin_theta = std::sqrt(std::max(0.0, 1.0 - cos_theta*cos_theta));

    double phi = 2.0 * M_PI * u.y;
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    return {sin_theta*cos_phi, sin_theta*sin_phi, cos_theta};
}

double Sampler::UniformSpherePdf()
{
    constexpr double pdf = 1.0 / (4.0 * M_PI);
    return pdf;
}

///
/// @brief Sample a unit hemisphere using a uniform distribution.
///
math::vec3d Sampler::UniformHemisphere(const math::vec2d &u)
{
    double cos_theta = u.x;
    double sin_theta = std::sqrt(std::max(0.0, 1.0 - cos_theta*cos_theta));

    double phi = 2.0 * M_PI * u.y;
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    return {sin_theta*cos_phi, sin_theta*sin_phi, cos_theta};
}

double Sampler::UniformHemispherePdf()
{
    constexpr double pdf = 0.5 * M_1_PI;    // 2.0/(4.0*M_PI)
    return pdf;
}

///
/// @brief Sample a unit hemisphere using a cosine distribution.
///
math::vec3d Sampler::CosineHemisphere(const math::vec2d &u)
{
    double cos_theta = std::sqrt(u.x);
    double sin_theta = std::sqrt(1.0 - u.x);

    double phi = 2.0 * M_PI * u.y;
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    return {sin_theta*cos_phi, sin_theta*sin_phi, cos_theta};
}

double Sampler::CosineHemispherePdf(const double cos_theta)
{
    return cos_theta > 0.0 ? cos_theta * M_1_PI : 0.0;
}

///
/// @brief Sample a cone using a uniform distribution.
///
math::vec3d Sampler::UniformCone(
    const math::vec2d &u,
    const double cos_theta_max)
{
    double cos_theta = 1.0 - u.x * (1.0 - cos_theta_max);
    double sin_theta = std::sqrt(std::max(0.0, 1.0 - cos_theta*cos_theta));

    double phi = 2.0 * M_PI * u.y;
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    return {sin_theta*cos_phi, sin_theta*sin_phi, cos_theta};
}

double Sampler::UniformConePdf(const double cos_theta_max)
{
    constexpr double pdf = 1.0 / (2.0 * M_PI);
    return pdf / (1.0 - cos_theta_max);
}

///
/// @brief Sample unit disk using a uniform distribution:
///  r = sqrt(u),
///  theta = 2*pi*u
///  pdf = 1 / pi;
///
math::vec2d Sampler::UniformDisk(const math::vec2d &u)
{
    return math::vec2d{std::sqrt(u.x), 2.0 * M_PI * u.y};
}

double Sampler::UniformDiskPdf()
{
    constexpr double pdf = M_1_PI;
    return pdf;
}

///
/// @brief Sample unit triangle using a uniform distribution.
///
math::vec2d Sampler::UniformTriangle(const math::vec2d &u)
{
    double r = std::sqrt(u.x);
    return math::vec2d{1.0 - r, r*u.y};
}

double Sampler::UniformTrianglePdf()
{
    constexpr double pdf = 2.0;
    return pdf;
}
