#pragma once

#include <cstdint>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <vector>

#include <asserter/src/asserter.hpp>
#include <core_types/src/rect2.hpp>
#include <core_types/src/vec2.hpp>

namespace path_comp {

template <typename scalar_t = float, typename index_t = std::uint32_t>
struct Loop {
    using scalar_type = scalar_t;
    using index_type = index_t;
    using vector_type = ct::Vec2<scalar_type>;
    using rect_type = ct::Rect2<scalar_type>;

    Loop(vector_type begin);

    Loop& line(vector_type dest);
    Loop& curve(vector_type control, vector_type dest);
    Loop& curve(vector_type control1, vector_type control2, vector_type dest);
    Loop& close();

    std::span<const vector_type> points() const;
    std::span<const index_type> segments() const;
    rect_type bounds_control_points() const;
    rect_type bounds_end_points() const;

    template <typename S, typename I>
    Loop<S, I> convert_to() const;

private:
    template <typename S, typename I>
    friend struct Loop;

    Loop() = default;

    std::vector<vector_type>& points();
    std::vector<index_type>& segments();

    std::vector<vector_type> m_points;
    std::vector<index_type> m_segments;
    rect_type m_bounds_end_points;
    rect_type m_bounds_all;

    bool m_closed = false;
};

template <typename scalar_t, typename index_t = uint32_t>
path_comp::Loop<scalar_t, index_t> make_frame(ct::Rect2<scalar_t>);
} // path_comp

#include "loop_impl.hpp"
