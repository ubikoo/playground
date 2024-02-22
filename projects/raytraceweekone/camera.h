//
// camera.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef CAMERA_H_
#define CAMERA_H_

#include "common.h"
#include "ray.h"

struct Camera {
    math::orthod m_ortho;       // orthonormal basis set
    math::vec3d m_eye;          // eye position
    double m_width;             // -1 <= width <= 1
    double m_height;            // -1 <= height <= 1
    double m_depth;             // 0 <= depth

    Ray rayto(const math::vec2d &uv) const;

    static Camera Create(
        const math::vec3d &eye,
        const math::vec3d &ctr,
        const math::vec3d &up,
        double fov,
        double aspect);
};

#endif // CAMERA_H_
