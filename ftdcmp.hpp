#pragma once

#include <array>
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

template<typename T> 
using path_type = path_comp::Comp< path_comp::Loop< std::array<T, 2> > >;

std::function<path_type<long>(unsigned long)> make_decomposer(std::string font_file, unsigned font_index = 0);
std::function<path_type<float>(unsigned long)> make_decomposer_f(std::string font_file, unsigned font_index = 0);


} // ftdcmp
