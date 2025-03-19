
#pragma once

#include <iostream>
#include <asserter/src/asserter.hpp>


template <class T>
static std::ostream& write_vector(T p, std::ostream& o)
{
    o << p[0] << " " << p[1];
    return o;
}

template <typename T, typename U>
static void start_loop(const T& builder, U id, std::ostream& result)
{
    result << "<path id=\"" << id << "\" d=\"M";
    write_vector(builder.points()[0], result);
}

static void end_loop(std::ostream& result)
{
    result << "Z\"";
    result << "/>\n";
}

template <typename T, typename U>
static void end_segment(const T& points, U begin, U size, std::ostream& result)
{
    switch (size) {
    case 1:
        result << " L";
        break;
    case 2:
        result << " Q";
        break;
    case 3:
        result << " C";
        break;
    default:
        ASSERT(false)(begin)(size);
    }

    while (size) {
        result << " ";
        write_vector(points[begin % points.size()], result);
        --size;
        ++begin;
    }
}


template <typename T, typename U>
std::string make_html_path(const path_comp::Loop<T, U> & builder, std::string name)
{
    std::stringstream result;

    start_loop(builder, name, result);
    U segment_begin = 1;
    for (auto segment_size : builder.segments())
    {
        end_segment(builder.points(), segment_begin, segment_size, result);
        segment_begin += segment_size;
    }

    end_loop(result);
    return result.str();
}

template <typename T>
std::string make_html_path(const path_comp::Comp<T> & builder, std::string name)
{
    std::stringstream result;
    for (auto loop : builder.loops())
    {
        result << make_html_path(loop, name);
    }
    
    return result.str();
}

