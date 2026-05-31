#pragma once

#include <cmath>
#include <optional>
#include <sstream>
#include <string>

#include <asserter/src/asserter.hpp>

#include "vec2.hpp"

namespace ct {

template <typename T>
struct Vec3 {
    T x;
    T y;
    T z;

    Vec3() = default;

    Vec3(T x, T y, T z)
        : x(x)
        , y(y)
        , z(z)
    {
    }

    template <typename U>
    explicit Vec3(const Vec3<U>& other)
        : x(static_cast<T>(other.x))
        , y(static_cast<T>(other.y))
        , z(static_cast<T>(other.z))
    {
    }

    Vec2<T> xy() const
    {
        return { x, y };
    }

    Vec2<T> yz() const
    {
        return { y, z };
    }

    Vec2<T> xz() const
    {
        return { x, z };
    }

    Vec3& operator*=(const T& s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vec3 operator*(const T& s) const
    {
        Vec3 result(*this);
        result *= s;
        return result;
    }

    Vec3& operator/=(const T& s)
    {
        ASSERT(s != 0);
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    Vec3 operator/(const T& s) const
    {
        Vec3 result(*this);
        result /= s;
        return result;
    }

    Vec3& operator+=(const Vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3 operator+(const Vec3& v) const
    {
        Vec3 result(*this);
        result += v;
        return result;
    }

    Vec3& operator-=(const Vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vec3 operator-(const Vec3& v) const
    {
        Vec3 result(*this);
        result -= v;
        return result;
    }

    Vec3 operator-() const
    {
        return { -x, -y, -z };
    }

    bool operator==(const Vec3& v) const
    {
        return equal(*this, v);
    }

    bool operator!=(const Vec3& v) const
    {
        return !equal(*this, v);
    }

    std::string to_json_array(std::string name = "") const
    {
        std::stringstream result;

        if (!name.empty()) {
            result << "\"" << name << "\": ";
        }

        result << "[ " << x << ", " << y << ", " << z << " ]";
        return result.str();
    }

    std::string to_json() const
    {
        std::stringstream result;
        result << "{ \"x\": " << x << ", \"y\": " << y << ", \"z\": " << z << " }";
        return result.str();
    }
};

using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;
using Vec3u = Vec3<unsigned>;

template <typename T>
T dot(const Vec3<T>& v, const Vec3<T>& w)
{
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

template <typename T>
Vec3<T> cross(const Vec3<T>& v, const Vec3<T>& w)
{
    return {
        v.y * w.z - v.z * w.y,
        v.z * w.x - v.x * w.z,
        v.x * w.y - v.y * w.x
    };
}

template <typename T>
T length(const Vec3<T>& v)
{
    return sqrt(dot(v, v));
}

template <typename T>
T distance(const Vec3<T>& a, const Vec3<T>& b)
{
    return length(a - b);
}

template <typename T>
Vec3<T> normalize(const Vec3<T>& v)
{
    return v / length(v);
}

template <typename T, typename U>
bool equal(const Vec3<T>& a, const Vec3<T>& b, U tolerance)
{
    return equal(a.x, b.x, tolerance)
        && equal(a.y, b.y, tolerance)
        && equal(a.z, b.z, tolerance);
}

template <typename T>
bool equal(const Vec3<T>& a, const Vec3<T>& b)
{
    return equal(a.x, b.x)
        && equal(a.y, b.y)
        && equal(a.z, b.z);
}

template <typename T, typename U>
Vec3<T> max(const Vec3<T>& a, const Vec3<U>& b)
{
    return { std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z) };
}

template <typename T, typename U>
Vec3<T> min(const Vec3<T>& a, const Vec3<U>& b)
{
    return { std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z) };
}

template <typename T>
std::optional<Vec3<T>> intersect_with_plane(const Vec3<T>& line_point,
    const Vec3<T>& line_dir,
    const Vec3<T>& plane_point,
    const Vec3<T>& plane_normal,
    T line_plane_parallel_tolerance = T(0.0001))
{
    const auto td = dot(line_dir, plane_normal);
    if (equal(td, 0, line_plane_parallel_tolerance)) {
        return std::nullopt;
    }
    const auto tn = dot(plane_point - line_point, plane_normal);
    return line_point + line_dir * (tn / td);
}

template <typename T>
T distance_to_line(const Vec3<T>& line_point,
    const Vec3<T>& line_dir,
    const Vec3<T>& point)
{
    const auto intersection = intersect_with_plane(
        line_point,
        line_dir,
        point,
        line_dir);

    return distance(intersection.value_or(line_point), point);
}

} // ct
