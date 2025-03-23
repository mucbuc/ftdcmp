#include "ftdcmp.hpp"
#include <atomic>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_BBOX_H

#define FOO \
    std::cout << __PRETTY_FUNCTION__ << std::endl

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

template<typename T> 
struct PathInfo {

    using vector_type = std::array<T, 2>;
    using loop_type = path_comp::Loop< vector_type >;

    PathInfo(ftdcmp::vector_type size)
    : m_current()
    , m_path(size)
    {}

    std::shared_ptr< loop_type > m_current;
    path_comp::Comp< loop_type > m_path;

    static int
    moveTo(const FT_Vector* to,
        void* user)
    {
        PathInfo* path = reinterpret_cast<PathInfo*>(user);

        if (path->m_current) {
            path->m_path.insert(*path->m_current);
        }

        path->m_current = std::make_shared<loop_type>(vector_type { { T{to->x}, T{to->y} } });
        return 0;
    }

    static int
    lineTo(const FT_Vector* to,
        void* user)
    {
        PathInfo* path = reinterpret_cast<PathInfo*>(user);
        ASSERT(path->m_current);

        path->m_current->line(vector_type { { T{to->x}, T{to->y} } });
        return 0;
    }

    static int
    conicTo(const FT_Vector* control,
        const FT_Vector* to,
        void* user)
    {
        PathInfo* path = reinterpret_cast<PathInfo*>(user);
        ASSERT(path->m_current);
        path->m_current->curve(vector_type { { T{control->x}, T{control->y} } }, vector_type { { T{to->x}, T{to->y} } });
        return 0;
    }

    static int
    cubicTo(const FT_Vector* control1,
        const FT_Vector* control2,
        const FT_Vector* to,
        void* user)
    {
        PathInfo* path = reinterpret_cast<PathInfo*>(user);
        ASSERT(path->m_current);
        path->m_current->curve(vector_type { { T{control1->x, control1->y} } }, vector_type { { T{control2->x}, T{control2->y} } }, vector_type { { T{to->x}, T{to->y} } });
        return 0;
    }
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

template<typename T>
std::function<path_type<T>(unsigned long)> make_decomposer(std::string font_file, unsigned font_index)
{
    if (gStates.m_initialized) {
        FT_Face face;
        FT_Error error = FT_New_Face(gStates.m_library, font_file.c_str(), font_index, &face);

        if (!error) {

            return [face](auto symbol) {
                const int glyph_index = FT_Get_Char_Index(face, symbol);

                FT_Error error = FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);
                if (error) {
                    std::cerr << "[ftdcmp] failed to load glyph for symbol: " << symbol << " error:" << error << std::endl;
                    return path_type();
                }

                FT_GlyphSlot slot = face->glyph;

                FT_Outline& outline = slot->outline;

                if (slot->format != FT_GLYPH_FORMAT_OUTLINE) {
                    std::cerr << "[ftdcmp] not able able to process glyph format for symbol: " << symbol << std::endl;
                    return path_type<T>();
                }

                error = FT_Outline_Check(&outline);
                if (error) {
                    std::cerr << "[ftdcmp] failed to process glyph for symbol: " << symbol << " error: " << error << std::endl;
                    return path_type();
                }

                if (outline.n_contours <= 0 || outline.n_points <= 0) {
                    std::cerr << "[ftdcmp] glyph missing path data for symbol: " << symbol << std::endl;
                    return path_type<T>();
                }

                FT_Outline_Funcs outlineFuncs = {
                    moveTo<T>,
                    lineTo<T>,
                    conicTo<T>,
                    cubicTo<T>,
                    0, // no shift
                    0 // no delta
                };

                PathInfo result{{ T{slot->advance.x}, T{slot->advance.y}} };
                FT_Outline_Decompose(&outline, &outlineFuncs, &result);

                if (result.m_current) {
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
        return path_type<T>();
    };
};

} // ftdcmp
