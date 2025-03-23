#include <ftdcmp/ftdcmp.hpp>
#include <iostream>

#include "make_html_path.hpp"

int main()
{
    ftdcmp::init();

#ifdef __EMSCRIPTEN__
    auto dcmp = ftdcmp::make_decomposer_l("ArialUnicode.ttf");
#else
    auto dcmp = ftdcmp::make_decomposer_l("/Library/Fonts/Arial Unicode.ttf");
#endif

    std::cout << make_html_path(dcmp('a'), "container") << std::endl;
    std::cout << make_html_path(dcmp('B'), "container2") << std::endl;
    ftdcmp::release();

    return 0;
}
