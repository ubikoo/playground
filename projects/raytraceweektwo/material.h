//
// material.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <cstdint>
#include "common.h"
#include "color.h"

struct Material {
    // Material properties.
    enum : uint32_t {
        Diffuse = 0,
        Conductor,
        Dielectric
    };
    uint32_t type;          // material type
    Color rho;              // reflectance
    double ior;             // index of refraction
    Color Le;               // emitted radiance

    // Diffuse material.
    static Material CreateDiffuse(const Color &rho);

    // Conductor material.
    static Material CreateConductor(const Color &rho);

    // Dielectric material.
    static Material CreateDielectric(const double ior);
};

#endif // MATERIAL_H_
