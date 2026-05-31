namespace path_comp {

template <typename scalar_t, typename index_t>
Loop<scalar_t, index_t>::Loop(vector_type begin)
    : m_points(1, begin)
    , m_segments()
    , m_bounds_end_points()
    , m_bounds_all()
{
    m_bounds_end_points.expand(begin);
    m_bounds_all.expand(begin);
}

template <typename scalar_t, typename index_t>
auto Loop<scalar_t, index_t>::line(vector_type dest) -> Loop&
{
    ASSERT(!m_points.empty());
    ASSERT(!m_closed);

    points().push_back(dest);
    segments().push_back(points().size());

    m_bounds_end_points.expand(dest);
    m_bounds_all.expand(dest);

    return *this;
}

template <typename scalar_t, typename index_t>
auto Loop<scalar_t, index_t>::curve(vector_type control, vector_type dest) -> Loop&
{
    ASSERT(!m_points.empty());
    ASSERT(!m_closed);

    points().push_back(control);
    points().push_back(dest);
    segments().push_back(points().size());

    m_bounds_end_points.expand(dest);
    m_bounds_all.expand(control).expand(dest);

    return *this;
}

template <typename scalar_t, typename index_t>
auto Loop<scalar_t, index_t>::curve(vector_type control1, vector_type control2, vector_type dest) -> Loop&
{
    ASSERT(!m_points.empty());
    ASSERT(!m_closed);

    points().push_back(control1);
    points().push_back(control2);
    points().push_back(dest);
    segments().push_back(points().size());

    m_bounds_end_points.expand(dest);
    m_bounds_all.expand(control1).expand(control2).expand(dest);

    return *this;
}

template <typename scalar_t, typename index_t>
auto Loop<scalar_t, index_t>::close() -> Loop&
{
    ASSERT(!points().empty());
    if (!m_closed) {
        segments().push_back(points().size() + 1);
        m_closed = true;
    }

    return *this;
}

template <typename scalar_t, typename index_t>
auto Loop<scalar_t, index_t>::points() -> std::vector<vector_type>&
{
    return m_points;
}

template <typename scalar_t, typename index_t>
auto Loop<scalar_t, index_t>::points() const -> std::span<const vector_type>
{
    return m_points;
}

template <typename scalar_t, typename index_t>
auto Loop<scalar_t, index_t>::segments() -> std::vector<index_t>&
{
    return m_segments;
}

template <typename scalar_t, typename index_t>
auto Loop<scalar_t, index_t>::segments() const -> std::span<const index_t>
{
    return m_segments;
}

template <typename scalar_t, typename index_t>
template <typename S, typename I>
Loop<S, I> Loop<scalar_t, index_t>::convert_to() const
{
    Loop<S, I> result;

    result.m_bounds_all = m_bounds_all;
    result.m_bounds_end_points = m_bounds_end_points;
    result.m_closed = m_closed;

    result.points().reserve(points().size());
    for (auto point : points()) {
        result.points().push_back(ct::Vec2<S> { point });
    }

    result.segments().reserve(segments().size());
    for (auto segments : segments()) {
        result.segments().push_back(segments);
    }

    return result;
}

template <typename scalar_t, typename index_t>
auto Loop<scalar_t, index_t>::bounds_control_points() const -> rect_type
{
    return m_bounds_all;
}

template <typename scalar_t, typename index_t>
auto Loop<scalar_t, index_t>::bounds_end_points() const -> rect_type
{
    return m_bounds_end_points;
}

template <typename scalar_t, typename index_t>
path_comp::Loop<scalar_t, index_t> make_frame(ct::Rect2<scalar_t> bounds)
{
    using loop_type = path_comp::Loop<scalar_t, index_t>;

    const auto result = loop_type { { bounds.left(), bounds.top() } }
                            .line({ bounds.left(), bounds.bottom() })
                            .line({ bounds.right(), bounds.bottom() })
                            .line({ bounds.right(), bounds.top() })
                            .close();
    return result;
}

} // path_comp
