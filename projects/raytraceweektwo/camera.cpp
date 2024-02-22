//
// camera.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "common.h"
#include "camera.h"
#include "ray.h"
#include "sampler.h"

///
/// @brief Create a camera at a specified eye position, looking into ctr, with
/// vertical up direction, and with a field of view fov and aspect ratio.
/// The camera focus plane is located at a distance given by its depth. This
/// value is fixed, so both the width and height are scaled to maintain the
/// fov angle invariant in the scaling.
///
Camera Camera::Create(
    const math::vec3d &eye,
    const math::vec3d &ctr,
    const math::vec3d &up,
    double fov,
    double aspect,
    double focus,
    double aperture)
{
    Camera camera;

    camera.m_ortho = math::orthod::create_from_wv(eye - ctr, up);
    camera.m_eye = eye;

    double half_theta = 0.5 * fov * M_PI / 180.0;
    double viewport_height = 2.0 * std::tan(half_theta);
    double viewport_width = aspect * viewport_height;

    camera.m_width = focus * viewport_width;
    camera.m_height = focus * viewport_height;
    camera.m_depth = focus;
    camera.m_radius = 0.5 * aperture;

    return camera;
}

///
/// @brief Generate a camera ray with direction towards a point on the screen
/// specified by the normalized coordinates (0,0) <= (u,v) <= (1,1).
///
/// Generate a new camera ray with direction towards a point on the screen
/// specified by the normalized coordinates (0,0) <= (u,v) <= (1,1).
/// The argument u2 represents two uniform random numbers used to sample a
/// uniform disk.
///
Ray Camera::rayto(const math::vec2d &u1, const math::vec2d &u2) const
{
    // Clamp the normalized coordinates to [0,1] range.
    double u = std::min(std::max(u1.x, 0.0), 1.0);
    double v = std::min(std::max(u1.y, 0.0), 1.0);

    // Sample camera offset in the unit disk and projected to world space.
    math::vec2d disk = Sampler::UniformDisk(u2);
    math::vec3d offset_local{
        m_radius * disk.x * std::cos(disk.y),
        m_radius * disk.x * std::sin(disk.y),
        0.0};
    math::vec3d offset = m_ortho.local_to_world(offset_local);

    // Generate a point in camera space and project it to world space.
    math::vec3d point_camera{
        (u - 0.5) * m_width,
        (v - 0.5) * m_height,
        -m_depth};
    math::vec3d point_world = m_ortho.local_to_world(point_camera);

    // Return a ray with origin at the camera and direction d.
    return {m_eye + offset, math::normalize(point_world - offset)};
}
