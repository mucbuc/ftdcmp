# path_comp
path composition utility

# interface

```
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

private:
    std::vector<vector_type> m_points;
    std::vector<index_type> m_segments;
};

template <typename Vector_t, typename Index_t>
void reverse(Loop<Vector_t, Index_t>&);

template <typename Loop_t>
struct Comp {
    using loop_type = Loop_t;
    using vector_type = typename Loop_t::vector_type;

    Comp(vector_type size);
    Comp& insert(loop_type l);

    vector_type& size();
    vector_type size() const;

    std::vector<loop_type>& loops();
    std::vector<loop_type> loops() const;

private:
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

} // path_comp
```
