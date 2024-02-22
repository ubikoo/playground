//
// color.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COLOR_H_
#define COLOR_H_

///
/// @brief Color tuple with unbounded red, green, and blue channel values.
///
struct Color {
    // Color channels.
    union {
        double data[3];
        struct { double r, g, b; };
    };

    // Unary arithmetic vector operators.
    Color &operator+=(const Color &other);
    Color &operator-=(const Color &other);
    Color &operator*=(const Color &other);
    Color &operator/=(const Color &other);

    // Unary arithmetic scalar operators.
    Color &operator+=(const double scalar);
    Color &operator-=(const double scalar);
    Color &operator*=(const double scalar);
    Color &operator/=(const double scalar);

    // Unary plus/negation operators.
    Color operator+() const;
    Color operator-() const;

    // Static colours.
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;

    // Clamp the specified color.
    static Color Clamp(
        const Color &color,
        const double lo = 0.0,
        const double hi = 1.0);
};

/// ---------------------------------------------------------------------------
/// @brief Unary arithmetic vector operators.
///
inline Color &Color::operator+=(const Color &other)
{
    data[0] += other.data[0];
    data[1] += other.data[1];
    data[2] += other.data[2];
    return *this;
}

inline Color &Color::operator-=(const Color &other)
{
    data[0] -= other.data[0];
    data[1] -= other.data[1];
    data[2] -= other.data[2];
    return *this;
}

inline Color &Color::operator*=(const Color &other)
{
    data[0] *= other.data[0];
    data[1] *= other.data[1];
    data[2] *= other.data[2];
    return *this;
}

inline Color &Color::operator/=(const Color &other)
{
    data[0] /= other.data[0];
    data[1] /= other.data[1];
    data[2] /= other.data[2];
    return *this;
}

/// ---------------------------------------------------------------------------
/// @brief Unary arithmetic scalar operators.
///
inline Color &Color::operator+=(const double scalar)
{
    data[0] += scalar;
    data[1] += scalar;
    data[2] += scalar;
    return *this;
}

inline Color &Color::operator-=(const double scalar)
{
    data[0] -= scalar;
    data[1] -= scalar;
    data[2] -= scalar;
    return *this;
}

inline Color &Color::operator*=(const double scalar)
{
    data[0] *= scalar;
    data[1] *= scalar;
    data[2] *= scalar;
    return *this;
}

inline Color &Color::operator/=(const double scalar)
{
    data[0] /= scalar;
    data[1] /= scalar;
    data[2] /= scalar;
    return *this;
}

/// ---------------------------------------------------------------------------
/// @brief Unary plus/negation operators.
///
inline Color Color::operator+() const
{
    Color result(*this);
    return result;
}

inline Color Color::operator-() const
{
    Color result(*this);
    result *= (double) (-1);
    return result;
}

/// ---------------------------------------------------------------------------
/// @brief Binary arithmetic operators between two vectors.
///
inline Color operator+(Color lhs, const Color &rhs)
{
    lhs += rhs;
    return lhs;
}

inline Color operator-(Color lhs, const Color &rhs)
{
    lhs -= rhs;
    return lhs;
}

inline Color operator*(Color lhs, const Color &rhs)
{
    lhs *= rhs;
    return lhs;
}

inline Color operator/(Color lhs, const Color &rhs)
{
    lhs /= rhs;
    return lhs;
}

/// ---------------------------------------------------------------------------
/// @brief Binary arithmetic operators between a vector and a scalar.
///
inline Color operator+(Color lhs, const double scalar)
{
    lhs += scalar;
    return lhs;
}

inline Color operator-(Color lhs, const double scalar)
{
    lhs -= scalar;
    return lhs;
}

inline Color operator*(Color lhs, const double scalar)
{
    lhs *= scalar;
    return lhs;
}

inline Color operator/(Color lhs, const double scalar)
{
    lhs /= scalar;
    return lhs;
}

/// ---------------------------------------------------------------------------
/// @brief Binary arithmetic operators between a scalar and a vector. Division is
/// not implemented, because its not commutative.
///
inline Color operator+(const double scalar, Color rhs)
{
    rhs += scalar;
    return rhs;
}

inline Color operator-(const double scalar, Color rhs)
{
    rhs -= scalar;
    return rhs;
}

inline Color operator*(const double scalar, Color rhs)
{
    rhs *= scalar;
    return rhs;
}

inline Color operator/(const double scalar, Color rhs)
{
    rhs /= scalar;
    return rhs;
}

#endif // COLOR_H_
