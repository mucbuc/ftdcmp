namespace path_comp {

template <typename Loop_t>
Composition<Loop_t>::Composition(const vector_type& advance)
    : m_loops()
    , m_bounds_end_points()
    , m_bounds_all()
    , m_advance(advance)
{
}

template <typename Loop_t>
Composition<Loop_t>& Composition<Loop_t>::insert(const loop_type& l)
{
    m_loops.push_back(l);
    m_bounds_all.expand(l.bounds_control_points());
    m_bounds_end_points.expand(l.bounds_end_points());
    return *this;
}

template <typename Loop_t>
auto Composition<Loop_t>::bounds_control_points() const -> rect_type
{
    return m_bounds_all;
}

template <typename Loop_t>
auto Composition<Loop_t>::bounds_end_points() const -> rect_type
{
    return m_bounds_end_points;
}

template <typename Loop_t>
auto Composition<Loop_t>::advance() const -> const vector_type&
{
    return m_advance;
}

template <typename Loop_t>
auto Composition<Loop_t>::advance() -> vector_type&
{
    return m_advance;
}

template <typename Loop_t>
auto Composition<Loop_t>::loops() const -> std::span<const loop_type>
{
    return m_loops;
}

template <typename Loop_t>
template <typename S, typename I>
Composition<Loop<S, I>> Composition<Loop_t>::convert_to() const
{
    Composition<Loop<S, I>> result;
    result.m_loops.reserve(m_loops.size());
    for (auto loop : m_loops) {
        result.insert(loop.template convert_to<S, I>());
    }
    return result;
}

} // path_comp
