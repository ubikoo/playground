//
// material.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "color.h"
#include "material.h"

///
/// @brief Diffuse material factory function.
///
Material Material::CreateDiffuse(const Color &rho)
{
    return {
        Material::Diffuse,      // type
        rho,                    // rho
        0.0,                    // ior
        Color::Black,           // Le
    };
}

///
/// @brief Conductor material factory function.
///
Material Material::CreateConductor(const Color &rho)
{
    return {
        Material::Conductor,    // type
        rho,                    // rho
        0.0,                    // ior
        Color::Black,           // Le
    };
}

///
/// @brief Dielectric material factory function.
///
Material Material::CreateDielectric(const double ior)
{
    return {
        Material::Dielectric,    // type
        Color::Black,           // rho
        ior,                    // ior
        Color::Black,           // Le
    };
}
