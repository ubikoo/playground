//
// film.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef FILM_H_
#define FILM_H_

#include <vector>
#include "common.h"
#include "color.h"

///
/// @brief Maintain an array of pixels with a specified width and height.
///
struct Film {
    // Member variables.
    uint32_t m_width;
    uint32_t m_height;
    std::vector<Color> m_pixels;

    // Clear the film pixels.
    void clear();

    // Set the film pixel to the specified color.
    void set(const uint32_t x, const uint32_t y, const Color &color);

    // Add the specified color the film pixel.
    void add(const uint32_t x, const uint32_t y, const Color &color);

    // Get the specified color of the film pixel.
    const Color &get(const uint32_t x, const uint32_t y) const;

    // Sample a point in the film pixel using normalized coordinates.
    math::vec2d sample(
        const uint32_t x,
        const uint32_t y,
        const math::vec2d &u) const;

    // Factory function.
    static Film Create(const uint32_t width, const uint32_t height);
};

#endif // FILM_H_
