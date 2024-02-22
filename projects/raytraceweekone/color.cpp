//
// color.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <algorithm>
#include "color.h"

///
/// @brief Static colours.
///
const Color Color::Black = Color{0.0, 0.0, 0.0};
const Color Color::White = Color{1.0, 1.0, 1.0};
const Color Color::Red   = Color{1.0, 0.0, 0.0};
const Color Color::Green = Color{0.0, 1.0, 0.0};
const Color Color::Blue  = Color{0.0, 0.0, 1.0};

///
/// @brief Clamp the specified color.
///
Color Color::Clamp(const Color &color, const double lo, const double hi)
{
    return {
        std::min(std::max(color.r, lo), hi),
        std::min(std::max(color.g, lo), hi),
        std::min(std::max(color.b, lo), hi)
    };
}
