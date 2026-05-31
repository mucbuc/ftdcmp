#pragma once

#include "vec2.hpp"
#include <cmath>
#include <numbers>
#include <sstream>
#include <string>

namespace ct {

template <typename T>
struct Rect2 {
    Rect2()
        : m_min({ std::numeric_limits<T>::max(),
            std::numeric_limits<T>::max() })
        , m_max({ std::numeric_limits<T>::lowest(),
              std::numeric_limits<T>::lowest() })
    {
    }

    template <typename U>
    explicit Rect2(const Rect2<U>& other)
        : m_min(Vec2<T> { other.m_min })
        , m_max(Vec2<T> { other.m_max })
    {
    }

    Rect2& add_padding(const Vec2<T>& p)
    {
        expand(m_min - p);
        expand(m_max + p);

        return *this;
    }

    Rect2& add_padding(const auto& padding)
    {
        const auto p = Vec2<T> { padding, padding };
        add_padding(p);

        return *this;
    }

    Rect2& expand(const Vec2<T>& p)
    {
        m_min = min(m_min, p);
        m_max = max(m_max, p);

        return *this;
    }

    Rect2& expand(const Rect2& p)
    {
        m_min = min(m_min, p.m_min);
        m_max = max(m_max, p.m_max);

        return *this;
    }

    Vec2<T> size() const
    {
        return { width(), height() };
    }

    Vec2<T> center() const
    {
        return { (left() + right()) / 2, (top() + bottom()) / 2 };
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

    bool contains(const Vec2<T> point, bool exclude_boundry = false) const
    {
        return exclude_boundry ? left() < point.x && right() > point.x
                && top() < point.y && bottom() > point.y
                               : left() <= point.x && right() >= point.x
                && top() <= point.y && bottom() >= point.y;
    }

    Rect2 axis_swapped() const
    {
        Rect2 result;
        result.expand({ m_min.y,
                          m_min.x })
            .expand({ m_max.y,
                m_max.x });

        return result;
    }

    std::string to_json() const
    {
        std::stringstream result;
        result << "{ \"right\": " << right();
        result << ", \"left\": " << left();
        result << ", \"top\": " << top();
        result << ", \"bottom\": " << bottom() << " }";
        return result.str();
    }

    std::string to_svg_viewbox() const
    {
        std::stringstream view_box;
        view_box << left() << " " << top()
                 << " " << width() << " " << height();
        return view_box.str();
    }

private:
    Vec2<T> m_min;
    Vec2<T> m_max;
};

using Rect2f = Rect2<float>;
using Rect2i = Rect2<int>;
using Rect2u = Rect2<unsigned>;

template <typename T, typename... Args>
Rect2<T> make_Rect2(Args&&... args)
{
    static_assert((std::is_same_v<std::decay_t<Args>, Vec2<T>> && ...),
        "all arguments to make_Rect2 must be Vec2<T>");

    Rect2<T> result;
    ((result.expand(std::forward<Args>(args))), ...);

    return result;
}

template <class T>
std::ostream& operator<<(std::ostream& o, const Rect2<T>& rect)
{
    o << rect.to_json();
    return o;
}

} // ct
