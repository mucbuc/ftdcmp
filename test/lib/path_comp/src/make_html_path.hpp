#pragma once

#include "composition.hpp"
#include "loop.hpp"

namespace path_comp {

template <typename T, typename U>
std::string make_html_path(const path_comp::Loop<T, U>& loop, std::string name)
{
    std::stringstream result;

    const auto write_vector = [&result](const auto& p) {
        result << p.x << " " << p.y;
    };

    const auto start_loop
        = [write_vector, &result](const auto& loop, auto id) {
              ASSERT(!loop.points().empty());
              result << "<path id=\"" << id << "\" d=\"M ";
              write_vector(loop.points()[0]);
          };

    const auto end_segment
        = [write_vector, &result](const auto& points, auto begin, auto size) {
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
                  ASSERT(false)
                  (begin)(size);
              }

              while (size) {
                  result << " ";
                  write_vector(points[begin % points.size()]);
                  --size;
                  ++begin;
              }
          };

    start_loop(loop, name);
    auto segment_begin = 1;
    for (auto segment_end : loop.segments()) {
        const auto segment_size = segment_end - segment_begin;
        end_segment(loop.points(), segment_begin, segment_size);
        segment_begin = segment_end; // segment_size;
    }

    result << "\"/>";

    return result.str();
}

template <typename T>
std::string make_html_path(const path_comp::Composition<T>& comp, std::string name)
{
    std::stringstream result;
    for (auto loop_index = 0; loop_index < comp.loops().size(); ++loop_index) {
        result << make_html_path(comp.loops()[loop_index], name + std::to_string(loop_index));
    }

    return result.str();
}

} // path_comp
