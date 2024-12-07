#include <ftdcmp/ftdcmp.hpp>

#include <iostream>

int main()
{
    ftdcmp::init();

#ifdef __EMSCRIPTEN__
    auto dcmp = ftdcmp::make_decomposer("ArialUnicode.ttf");
#else
    auto dcmp = ftdcmp::make_decomposer("/Library/Fonts/Arial Unicode.ttf");
#endif

    std::cout << path_comp::make_html_path(dcmp('a'), "container") << std::endl;
    std::cout << path_comp::make_html_path(dcmp('B'), "container2") << std::endl;
    ftdcmp::release();

    return 0;
}
