/**
 * \file Vec2.cpp
 * \author Jonathan Holmes
 * \author Hyunwoo Yang
 * \date 2025 Fall
 * \par CS200 Computer Graphics I
 * \copyright DigiPen Institute of Technology
 */

#include "Vec2.hpp"
#include <cmath>
#include <iostream>
#include <istream>
#include <ostream>

namespace Math
{

    /*--------------------------This is ivec2---------------------------------*/

    // Constructors
    ivec2::ivec2(const vec2& v) noexcept : x(static_cast<int>(v.x)), y(static_cast<int>(v.y))
    {
    }

    // Stream operators
    std::ostream& operator<<(std::ostream& os, const ivec2& v)
    {
        os << v.x << ", " << v.y;
        return os;
    }

    std::istream& operator>>(std::istream& is, ivec2& v)
    {
        is >> v.x >> v.y;
        return is;
    }

    /*-------------------------------This is vec2---------------------------------------------------*/
    // Constructors

    vec2::vec2(const ivec2& v) noexcept : x(v.x), y(v.y)
    {
    }

    float vec2::Length() const
    {
        return static_cast<float>(std::sqrt(x * x + y * y));
    }

    vec2 vec2::Normalize() const
    {
        const float length = static_cast<float>(std::sqrt(x * x + y * y));

        if (length > 0.0f)
        {
            const float inv_length = 1.0f / length;
            return { x * inv_length, y * inv_length };
        }
        return *this;
    }

    // Stream operators
    std::ostream& operator<<(std::ostream& os, const vec2& v)
    {
        os << v.x << ", " << v.y;
        return os;
    }

    std::istream& operator>>(std::istream& is, vec2& v)
    {
        is >> v.x >> v.y;
        return is;
    }

}
