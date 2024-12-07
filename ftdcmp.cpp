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

struct PathInfo {
    PathInfo() = default;

    std::shared_ptr<ftdcmp::path_type::Loop> m_current;
    ftdcmp::path_type m_path;
};

using vector_type = std::array<long, 2>;

static int
moveTo(const FT_Vector* to,
    void* user)
{
    PathInfo* path = reinterpret_cast<PathInfo*>(user);

    if (path->m_current) {
        path->m_path.append(*path->m_current);
    }

    path->m_current = std::make_shared<ftdcmp::path_type::Loop>(vector_type { { to->x, to->y } }.data());
    return 0;
}

static int
lineTo(const FT_Vector* to,
    void* user)
{
    PathInfo* path = reinterpret_cast<PathInfo*>(user);
    ASSERT(path->m_current);

    path->m_current->line(vector_type { { to->x, to->y } }.data());
    return 0;
}

static int
conicTo(const FT_Vector* control,
    const FT_Vector* to,
    void* user)
{
    PathInfo* path = reinterpret_cast<PathInfo*>(user);
    ASSERT(path->m_current);
    path->m_current->curve(vector_type { { control->x, control->y } }.data(), vector_type { { to->x, to->y } }.data());
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
    path->m_current->curve(vector_type { { control1->x, control1->y } }.data(), vector_type { { control2->x, control2->y } }.data(), vector_type { { to->x, to->y } }.data());
    return 0;
}

static FT_Outline_Funcs outlineFuncs = {
    moveTo,
    lineTo,
    conicTo,
    cubicTo,
    0, // no shift
    0 // no delta
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

std::function<path_type(unsigned long)> make_decomposer(std::string font_file, unsigned font_index)
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
                    return path_type();
                }

                error = FT_Outline_Check(&outline);
                if (error) {
                    std::cerr << "[ftdcmp] failed to process glyph for symbol: " << symbol << " error: " << error << std::endl;
                    return path_type();
                }

                if (outline.n_contours <= 0 || outline.n_points <= 0) {
                    std::cerr << "[ftdcmp] glyph missing path data for symbol: " << symbol << std::endl;
                    return path_type();
                }

                PathInfo result;
                FT_Outline_Decompose(&outline, &outlineFuncs, &result);

                if (result.m_current) {
                    result.m_path.append(*result.m_current);
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

} // ftdcmp
