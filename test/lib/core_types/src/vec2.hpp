#pragma once

#include <cmath>
#include <sstream>
#include <string>

#include <asserter/src/asserter.hpp>

#include "scalars.hpp"

namespace ct {

template <typename T>
struct Vec2 {
    T x;
    T y;

    Vec2() = default;

    Vec2(T x, T y)
        : x(x)
        , y(y)
    {
    }

    template <typename U>
    explicit Vec2(U x, U y)
        : x(static_cast<T>(x))
        , y(static_cast<T>(y))
    {
    }

    template <typename U>
    explicit Vec2(const Vec2<U>& other)
        : x(static_cast<T>(other.x))
        , y(static_cast<T>(other.y))
    {
    }

    template <typename U>
    Vec2& operator*=(const U& s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    template <typename U>
    Vec2 operator*(const U& s) const
    {
        Vec2 r(*this);
        r *= s;
        return r;
    }

    template <typename U>
    Vec2& operator/=(const U& s)
    {
        x /= s;
        y /= s;
        return *this;
    }

    template <typename U>
    Vec2 operator/(const U& s) const
    {
        Vec2 r(*this);
        r /= s;
        return r;
    }

    Vec2& operator+=(const Vec2& w)
    {
        x += w.x;
        y += w.y;
        return *this;
    }

    Vec2 operator+(const Vec2& w) const
    {
        Vec2 r(*this);
        r += w;
        return r;
    }

    Vec2& operator-=(const Vec2& w)
    {
        x -= w.x;
        y -= w.y;
        return *this;
    }

    Vec2 operator-(const Vec2& w) const
    {
        Vec2 r(*this);
        r -= w;
        return r;
    }

    Vec2 operator-() const
    {
        return { -x, -y };
    }

    bool operator==(const Vec2& v) const
    {
        return equal(*this, v);
    }

    bool operator!=(const Vec2& v) const
    {
        return !equal(*this, v);
    }

    std::string to_json_array(std::string name = "") const
    {
        std::stringstream result;

        if (!name.empty()) {
            result << "\"" << name << "\": ";
        }

        result << "[ " << x << ", " << y << " ]";
        return result.str();
    }

    std::string to_json() const
    {
        std::stringstream result;
        result << "{ \"x\": " << x << ", \"y\": " << y << " }";
        return result.str();
    }
};

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned>;

template <typename T>
Vec2<T> perp_left(const Vec2<T>& v)
{
    return { -v.y, v.x };
}

template <typename T>
Vec2<T> perp_right(const Vec2<T>& v)
{
    return { v.y, -v.x };
}

template <typename T>
T dot(const Vec2<T>& v, const Vec2<T>& w)
{
    return v.x * w.x + v.y * w.y;
}

template <typename T>
T length(const Vec2<T>& v)
{
    return std::sqrt(dot(v, v));
}

template <typename T>
T distance(const Vec2<T>& a, const Vec2<T>& b)
{
    return length(a - b);
}

template <typename T>
Vec2<T> normalize(const Vec2<T>& v)
{
    return v / length(v);
}

template <typename T, typename U>
bool equal(const Vec2<T>& a, const Vec2<T>& b, U tolerance)
{
    return equal(a.x, b.x, tolerance) && equal(a.y, b.y, tolerance);
}

template <typename T>
bool equal(const Vec2<T>& a, const Vec2<T>& b)
{
    return equal(a.x, b.x) && equal(a.y, b.y);
}

template <typename T, typename U>
Vec2<T> max(const Vec2<T>& a, const Vec2<U>& b)
{
    return { std::max(a.x, b.x), std::max(a.y, b.y) };
}

template <typename T, typename U>
Vec2<T> min(const Vec2<T>& a, const Vec2<U>& b)
{
    return { std::min(a.x, b.x), std::min(a.y, b.y) };
}

template <typename T>
std::ostream& operator<<(std::ostream& o, const Vec2<T>& v)
{
    o << v.to_json();
    return o;
}

template <typename T>
T distance_to_line(Vec2<T> line_point, Vec2<T> line_dir, const Vec2<T>& point)
{
    const auto unit_normal = normalize(perp_left(line_dir));
    const auto diff = line_point - point;
    return std::abs(dot(unit_normal, diff));
}

} // ct
