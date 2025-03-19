#pragma once

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <array>

#include <cassert>
#define ASSERT(p) assert((p))

#include <path_comp/src/interface.hpp>

namespace ftdcmp {
void init();
void release();

using vector_type = std::array<long, 2>;
using loop_type = path_comp::Loop<vector_type>;
using path_type = path_comp::Comp<loop_type>;
std::function<path_type(unsigned long)> make_decomposer(std::string font_file, unsigned font_index = 0);
} // ftdcmp
