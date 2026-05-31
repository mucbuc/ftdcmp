#pragma once

#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <vector>

#include "loop.hpp"
#include <asserter/src/asserter.hpp>
#include <core_types/src/rect2.hpp>

namespace path_comp {

template <typename loop_t>
struct Composition {
    using loop_type = loop_t;
    using vector_type = typename loop_type::vector_type;
    using scalar_type = typename loop_type::scalar_type;
    using index_type = typename loop_type::index_type;
    using rect_type = typename loop_type::rect_type;

    Composition& insert(const loop_type& l);

    std::span<const loop_type> loops() const;

    template <typename S, typename I>
    Composition<Loop<S, I>> convert_to() const;

    rect_type bounds_control_points() const;
    rect_type bounds_end_points() const;

private:
    template <typename T>
    friend struct Composition;

    std::vector<loop_type> m_loops;
    rect_type m_bounds_end_points;
    rect_type m_bounds_all;
};

} // path_comp

#include "composition_impl.hpp"
