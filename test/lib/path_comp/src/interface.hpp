#pragma once

#include <vector>
#include <string> 
#include <sstream>
#include <iostream>

#include <asserter/src/asserter.hpp>

#include <iostream>
#include <asserter/src/asserter.hpp>

namespace path_comp {

template <typename Vector_t, typename Index_t = uint32_t>
struct Loop {
    using scalar_type = typename Vector_t::value_type;
    using vector_type = Vector_t;
    using index_type = Index_t;

    Loop(vector_type begin);
    Loop& line(vector_type dest);
    Loop& curve(vector_type control, vector_type dest);
    Loop& curve(vector_type control1, vector_type control2, vector_type dest);
    Loop& close(); // legacy nop

    std::vector<vector_type>& points();
    std::vector<vector_type> points() const;
    std::vector<index_type>& segments();
    std::vector<index_type> segments() const;

    template <typename C>
    Loop<C, Index_t> convert_to() const;

private:
    template <typename C, typename I>
    friend struct Loop;

    Loop() = default;

    std::vector<vector_type> m_points;
    std::vector<index_type> m_segments;
};

template <typename Vector_t, typename Index_t>
void reverse(Loop<Vector_t, Index_t>&);

template <typename Loop_t>
struct Comp {
    using loop_type = Loop_t;
    using vector_type = typename Loop_t::vector_type;

    Comp(vector_type size = { 0, 0 });
    Comp& insert(loop_type l);

    vector_type& size();
    vector_type size() const;

    std::vector<loop_type>& loops();
    std::vector<loop_type> loops() const;

    template <typename C>
    Comp<Loop<C>> convert_to() const;

private:
    template <typename C>
    friend struct Comp;

    vector_type m_size;
    std::vector<loop_type> m_loops;
};

template <typename Vector_t>
struct Bounds {

    using vector_type = Vector_t;

    vector_type m_min;
    vector_type m_max;

    bool operator>(vector_type) const;
    bool operator>=(vector_type) const;
    bool operator<(vector_type) const;
    bool operator<=(vector_type) const;
};

template <typename Vector_t, typename Index_t = uint32_t>
path_comp::Loop<Vector_t, Index_t> make_frame(Bounds<Vector_t>, typename Vector_t::value_type offset = 0);

template <typename T, typename U>
std::string make_html_path(const path_comp::Loop<T, U> & builder, std::string name = "svg");

template <typename T>
std::string make_html_path(const path_comp::Comp<T> & builder, std::string name = "svg");

#include "impl.hpp"
#include "make_html_path.hpp"

} // path_comp
