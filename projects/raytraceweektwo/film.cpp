//
// film.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <vector>
#include <cstdint>
#include "common.h"
#include "color.h"
#include "film.h"

///
/// @brief Create a film with a specified width and height in pixels.
///
Film Film::Create(const uint32_t width, const uint32_t height)
{
    Film film;
    film.m_width = width;
    film.m_height = height;
    film.m_pixels.resize(width * height, Color::Black);
    return film;
}

///
/// @brief Clear the film pixels.
///
void Film::clear()
{
    std::fill(m_pixels.begin(), m_pixels.end(), Color::Black);
}

///
/// @brief Set the film pixel to the specified color.
///
void Film::set(const uint32_t x, const uint32_t y, const Color &color)
{
    m_pixels[x + y * m_width] = color;
}

///
/// @brief Add the specified color the film pixel.
///
void Film::add(const uint32_t x, const uint32_t y, const Color &color)
{
    m_pixels[x + y * m_width] += color;
}

///
/// @brief Get the specified color of the film pixel.
///
const Color &Film::get(const uint32_t x, const uint32_t y) const
{
    return m_pixels[x + y * m_width];
}

///
/// @brief Sample a point in the film pixel using normalized coordinates.
/// @note For simplicity, return only the centre position of the pixel given
/// by x and y.
///
math::vec2d Film::sample(
    const uint32_t x,
    const uint32_t y,
    const math::vec2d &u) const
{
    const double w = (double) m_width;
    const double h = (double) m_height;
    return math::vec2d{((double) x + u.x) / w, ((double) y + u.y) / h};
}
