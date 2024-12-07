#pragma once

#include <functional>
#include <memory>
#include <sstream>
#include <string>

#include <cassert>
#define ASSERT(p) assert((p))

#include <path_comp/src/interface.hpp>

namespace ftdcmp {
void init();
void release();

using path_type = path_comp::Builder<long, 2>;
std::function<path_type(unsigned long)> make_decomposer(std::string font_file, unsigned font_index = 0);
} // ftdcmp
