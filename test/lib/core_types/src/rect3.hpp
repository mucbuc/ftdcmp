#pragma once

#include "vec3.hpp"
#include <cmath>
#include <numbers>
#include <sstream>
#include <string>

namespace ct {

template <typename T>
struct Rect3 {

    Rect3()
        : m_min({ std::numeric_limits<T>::max(),
            std::numeric_limits<T>::max(),
            std::numeric_limits<T>::max() })
        , m_max({ std::numeric_limits<T>::lowest(),
              std::numeric_limits<T>::lowest(),
              std::numeric_limits<T>::lowest() })
    {
    }

    template <typename U>
    explicit Rect3(const Rect3<U>& other)
        : m_min(Vec3<T> { other.m_min })
        , m_max(Vec3<T> { other.m_max })
    {
    }

    Rect3& add_padding(const auto& padding)
    {
        const auto p = Vec3<T> { padding, padding, padding };
        expand(m_min - p);
        expand(m_max + p);

        return *this;
    }

    Rect3& expand(const Vec3<T>& p)
    {
        m_min = min(m_min, p);
        m_max = max(m_max, p);

        return *this;
    }

    Vec3<T> size() const
    {
        return { width(), height(), depth() };
    }

    Vec3<T> center() const
    {
        return { (left() + right()) / 2, (top() + bottom()) / 2, (back() + front()) / 2 };
    }

    T width() const
    {
        ASSERT(right() >= left());
        return right() - left();
    }

    T height() const
    {
        ASSERT(bottom() >= top());
        return bottom() - top();
    }

    T depth() const
    {
        ASSERT(back() >= front());

        return back() - front();
    }

    T left() const
    {
        return m_min.x;
    }

    T top() const
    {
        return m_min.y;
    }

    T right() const
    {
        return m_max.x;
    }

    T bottom() const
    {
        return m_max.y;
    }

    T back() const
    {
        return m_max.z;
    }

    T front() const
    {
        return m_min.z;
    }

    bool contains(const Vec3<T> point, bool exclude_boundry = false) const
    {
        return exclude_boundry ? left() < point.x && right() > point.x
                && top() < point.y && bottom() > point.y
                && front() < point.z && back() > point.z
                               : left() <= point.x && right() >= point.x
                && top() <= point.y && bottom() >= point.y
                && front() <= point.z && back() >= point.z;
    }

    std::string to_json() const
    {
        std::stringstream result;
        result << "{ \"right\": " << right();
        result << ", \"left\": " << left();
        result << ", \"top\": " << top();
        result << ", \"bottom\": " << bottom();
        result << ", \"back\": " << back();
        result << ", \"front\": " << front() << " }";
        return result.str();
    }

private:
    Vec3<T> m_min;
    Vec3<T> m_max;
};

using Rect3f = Rect3<float>;
using Rect3i = Rect3<int>;
using Rect3u = Rect3<unsigned>;

template <typename T, typename... Args>
Rect3<T> make_Rect3(Args&&... args)
{
    static_assert((std::is_same_v<std::decay_t<Args>, Vec3<T>> && ...),
        "all arguments to make_Rect3 must be Vec3<T>");

    Rect3<T> result;
    ((result.expand(std::forward<Args>(args))), ...);

    return result;
}

template <class T>
std::ostream& operator<<(std::ostream& o, const Rect3<T>& rect)
{
    o << rect.to_json();
    return o;
}

} // ct
