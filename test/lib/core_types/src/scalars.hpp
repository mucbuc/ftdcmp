#pragma once

#include <cstdlib>

namespace ct {

template <typename T, typename U>
bool equal(const T& a, const T& b, U tolerance = 0)
{
    return std::abs(a - b) <= tolerance;
}

template <typename T>
bool equal(const T& a, const T& b)
{
    return a == b;
}

} // ct
