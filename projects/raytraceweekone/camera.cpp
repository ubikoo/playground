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

///
/// @brief Create a camera at a specified eye position, looking at ctr,
/// with up-direction, with a field of view fov and aspect ratio.
///
Camera Camera::Create(
    const math::vec3d &eye,
    const math::vec3d &ctr,
    const math::vec3d &up,
    double fov,
    double aspect)
{
    Camera camera;

    camera.m_ortho = math::orthod::create_from_wv(eye - ctr, up);
    camera.m_eye = eye;

    double half_width = 1.0;
    double half_height = 1.0;
    if (aspect < 1.0) {
        half_width *= aspect;
    } else {
        half_height /= aspect;
    }
    double half_theta = 0.5 * fov * M_PI / 180.0;

    camera.m_width = 2.0 * half_width;
    camera.m_height = 2.0 * half_height;
    camera.m_depth = half_height / std::tan(half_theta);

    return camera;
}

///
/// @brief Generate a camera ray with direction towards a point on the screen
/// specified by the normalized coordinates (0,0) <= (u,v) <= (1,1).
///
Ray Camera::rayto(const math::vec2d &uv) const
{
    // Clamp the normalized coordinates to [0,1] range.
    double u = std::min(std::max(uv.x, 0.0), 1.0);
    double v = std::min(std::max(uv.y, 0.0), 1.0);

    // Generate a point in camera space and project it to world space
    math::vec3d point_camera{
        (u - 0.5) * m_width,
        (v - 0.5) * m_height,
        -m_depth};
    math::vec3d point_world = m_ortho.local_to_world(point_camera);

    // Return a ray with origin at the camera and direction d.
    return {m_eye, math::normalize(point_world)};
}
