#include "ftdcmp.hpp"

#include <asserter/src/asserter.hpp>

#include <atomic>
#include <iostream>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_BBOX_H

namespace {

static struct
{
    std::atomic<bool> m_initialized = false;
    FT_Library m_library;

    FT_Error init_lib()
    {
        return FT_Init_FreeType(&m_library);
    }

} gStates;

template <typename T>
struct PathInfo {

    using vector_type = ftdcmp::vector_type;
    using loop_type = path_comp::Loop<vector_type>;

    PathInfo(const T& w, const T& h)
        : m_current()
        , m_path(vector_type{ w, h })
    {
    }

    std::shared_ptr<loop_type> m_current;
    path_comp::Comp<loop_type> m_path;

    static int
    moveTo(const FT_Vector* to,
        void* user)
    {
        PathInfo* path = reinterpret_cast<PathInfo*>(user);

        if (path->m_current) {
            path->m_current->close();
            path->m_path.insert(*path->m_current);
        }

        path->m_current = std::make_shared<loop_type>(vector_type { { T(to->x), T(to->y) } });
        return 0;
    }

    static int
    lineTo(const FT_Vector* to,
        void* user)
    {
        PathInfo* path = reinterpret_cast<PathInfo*>(user);
        ASSERT(path->m_current.get());

        path->m_current->line(vector_type { { T(to->x), T(to->y) } });
        return 0;
    }

    static int
    conicTo(const FT_Vector* control,
        const FT_Vector* to,
        void* user)
    {
        PathInfo* path = reinterpret_cast<PathInfo*>(user);
        ASSERT(path->m_current.get());
        path->m_current->curve(vector_type { { T(control->x), T(control->y) } }, vector_type { { T(to->x), T(to->y) } });
        return 0;
    }

    static int
    cubicTo(const FT_Vector* control1,
        const FT_Vector* control2,
        const FT_Vector* to,
        void* user)
    {
        PathInfo* path = reinterpret_cast<PathInfo*>(user);
        ASSERT(path->m_current.get());
        path->m_current->curve(vector_type { { T(control1->x), T(control1->y) } }, vector_type { { T(control2->x), T(control2->y) } }, vector_type { { T(to->x), T(to->y) } });
        return 0;
    }
};

struct FT_Face_Handle {
    ~FT_Face_Handle()
    {
        FT_Done_Face(m_face);
    }

    FT_Face m_face;
};

using path_type = ftdcmp::path_type;

std::function<path_type(unsigned long)> make_decomposer(std::string font_file, unsigned font_index)
{
    if (gStates.m_initialized) {

        auto face_owner_outside = std::make_shared<FT_Face_Handle>();

        FT_Error error = FT_New_Face(gStates.m_library, font_file.c_str(), font_index, &face_owner_outside->m_face);

        if (!error) {

            return [face_owner = face_owner_outside](auto symbol) {
                const int glyph_index = FT_Get_Char_Index(face_owner->m_face, symbol);

                FT_Error error = FT_Load_Glyph(face_owner->m_face, glyph_index, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);
                if (error) {
                    std::cerr << "[ftdcmp] failed to load glyph for symbol: " << symbol << " error:" << error << std::endl;
                    return path_type();
                }

                FT_GlyphSlot slot = face_owner->m_face->glyph;

                FT_Outline& outline = slot->outline;

                if (slot->format != FT_GLYPH_FORMAT_OUTLINE) {
                    std::cerr << "[ftdcmp] not able able to process glyph format for symbol: " << symbol << std::endl;
                    return path_type();
                }

                error = FT_Outline_Check(&outline);
                if (error) {
                    std::cerr << "[ftdcmp] failed to process glyph for symbol: " << symbol << " error: " << error << std::endl;
                    return path_type();
                }

                auto result = PathInfo<ftdcmp::float32_t>{ ftdcmp::float32_t(slot->advance.x), ftdcmp::float32_t(slot->advance.y) };
                if (outline.n_contours <= 0 || outline.n_points <= 0) {
                
                    if (slot->advance.x || slot->advance.y) {
                        std::cout << "[ftdcmp] glyph without contour or points: " << result.m_path.size()[0] << ", " << result.m_path.size()[1] << std::endl;
                        return result.m_path;
                    }

                    std::cerr << "[ftdcmp] glyph missing path data for symbol: " << symbol << std::endl;
                    return path_type();
                }

                FT_Outline_Funcs outlineFuncs = {
                    PathInfo<ftdcmp::float32_t>::moveTo,
                    PathInfo<ftdcmp::float32_t>::lineTo,
                    PathInfo<ftdcmp::float32_t>::conicTo,
                    PathInfo<ftdcmp::float32_t>::cubicTo,
                    0, // no shift
                    0 // no delta
                };
                
                FT_Outline_Decompose(&outline, &outlineFuncs, &result);

                for (auto& loop : result.m_path.loops()) {
                    loop.close();
                }

                if (result.m_current) {
                    result.m_current->close();
                    result.m_path.insert(*result.m_current);
                    result.m_current.reset();
                }

                return result.m_path;
            };
        } else {
            std::cerr << "[ftdcmp] failed to load font. error: " << error << " path: " << font_file << std::endl;
        }
    }
    return [](auto symbol) {
        return path_type();
    };
};

}

namespace ftdcmp {
void init()
{
    if (!gStates.m_initialized) {
        if (const auto error = gStates.init_lib()) {
            std::cout << "freetype init failed with error " << error << std::endl;
        } else {
            gStates.m_initialized = true;
        }
    }
}

void release()
{
    if (gStates.m_initialized) {
        gStates.m_initialized = false;

        FT_Done_FreeType(gStates.m_library);
    }
}

#pragma mark --

struct Font_Batch_Loader::Pimpl
{
    Pimpl(std::string font_path, unsigned font_index)
    : m_decomposer(make_decomposer_f(font_path, font_index))
    , m_points()
    , m_segments()
    , m_loops()
    , m_glyphs()
    {}

    int get_glyph_shader_index(unsigned long g)
    {
        const auto pos = m_glyph_map.find(g);
        if (pos != m_glyph_map.end())
        {
            return pos->second;
        }
        return -1;
    }

    void load_glyph(unsigned long g)
    {
        auto glyph = m_decomposer(g);

        for (auto loop : glyph.loops())
        {
            std::cout << "insert loop at segment index " << m_segments.size() << std::endl;

            if (!m_points.empty()) {
                std::for_each(loop.segments().begin(), loop.segments().end(), [this](auto & s) {
                    s += m_points.size();
                });
            }
            
            m_segments.insert( m_segments.end(), loop.segments().begin(), loop.segments().end() );
            m_points.insert( m_points.end(), loop.points().begin(), loop.points().end() );
            m_loops.push_back( unsigned(m_segments.size()) );
        }
        m_glyph_map[g] = unsigned(m_glyphs.size());
        m_glyphs.push_back(unsigned(m_loops.size()));
    
        std::cout << "insert glyph " << g << " at loop index " << m_loops.size() << std::endl;
    }

    std::vector<vector_type> points() const
    {
        return m_points;
    }

    std::vector<uint32_t> segments() const
    {
        return m_segments;
    }

    std::vector<uint32_t> loops() const
    {
        return m_loops;
    }

    std::vector<uint32_t> glyphs() const
    {
        return m_glyphs;
    }

    std::function<path_type(unsigned long)> m_decomposer;
    std::vector<vector_type> m_points;
    std::vector<uint32_t> m_segments;
    std::vector<uint32_t> m_loops;
    std::vector<uint32_t> m_glyphs;
    std::map<unsigned long, uint32_t> m_glyph_map;
};

Font_Batch_Loader::Font_Batch_Loader(std::shared_ptr<Pimpl> p)
: m_pimpl(p)
{}

void Font_Batch_Loader::load_glyph(unsigned long glyph)
{
    m_pimpl->load_glyph(glyph);
}

int Font_Batch_Loader::get_glyph_shader_index(unsigned long glyph)
{
    return m_pimpl->get_glyph_shader_index(glyph);
}

void Font_Batch_Loader::load_glyphs(std::string glyphs)
{
    for (auto c : glyphs) {
        m_pimpl->load_glyph(c);
    }
}

void Font_Batch_Loader::load_glyphs(std::span<unsigned long> glyphs)
{
    for (auto c : glyphs) {
        m_pimpl->load_glyph(c);
    }
}

Font_Batch_Loader Font_Batch_Loader::make(std::string font_file, unsigned font_index)
{
    return std::make_shared<Pimpl>(font_file, font_index);
}

std::vector<vector_type> Font_Batch_Loader::points() const
{
    return m_pimpl->points();
}

std::vector<uint32_t> Font_Batch_Loader::segments() const
{
    return m_pimpl->segments();
}

std::vector<uint32_t> Font_Batch_Loader::loops() const
{
    return m_pimpl->loops();
}

std::vector<uint32_t> Font_Batch_Loader::glyphs() const
{
    return m_pimpl->glyphs();
}

#pragma mark -- 

// std::function<path_type<long>(unsigned long)> make_decomposer_l(std::string font_file, unsigned font_index)
// {
//     return make_decomposer<long>(font_file, font_index);
// }

std::function<path_type(unsigned long)> make_decomposer_f(std::string font_file, unsigned font_index)
{
    return make_decomposer(font_file, font_index);
}

} // ftdcmp
