#pragma once

#include <array>
#include <functional>
#include <memory>
#include <span>
#include <sstream>
#include <string>

#include <core_types/src/vec2.hpp>
#include <path_comp/src/composition.hpp>

namespace ftdcmp {
void init();
void release();

using float32_t = float;
using uint32_t = unsigned;

using vector_type = ct::Vec2f;
using loop_type = path_comp::Loop<>;
using path_type = path_comp::Composition<loop_type>;

struct Font_Batch_Loader {
    static Font_Batch_Loader make(std::string font_file, unsigned font_index = 0);

    void load_glyph(unsigned long);
    void load_glyphs(std::string);
    void load_glyphs(std::span<unsigned long>);

    int get_glyph_shader_index(unsigned long);

    std::vector<vector_type> points() const;
    std::vector<uint32_t> segments() const;
    std::vector<uint32_t> loops() const;
    std::vector<uint32_t> glyphs() const;

private:
    struct Pimpl;
    std::shared_ptr<Pimpl> m_pimpl;

    Font_Batch_Loader(std::shared_ptr<Pimpl>);
};

// std::function<path_type<long>(unsigned long)> make_decomposer_l(std::string font_file, unsigned font_index = 0);

std::function<path_type(unsigned long)> make_decomposer_f(std::string font_file, unsigned font_index = 0);

} // ftdcmp
