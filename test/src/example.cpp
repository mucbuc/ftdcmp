#include <ftdcmp/ftdcmp.hpp>
#include <iostream>

#include <asserter/src/asserter.hpp>

int main()
{
    ftdcmp::init();

#ifdef __EMSCRIPTEN__
    auto dcmp = ftdcmp::make_decomposer_l("ArialUnicode.ttf");
#else
    auto dcmp = ftdcmp::make_decomposer_l("/Library/Fonts/Arial Unicode.ttf");
#endif

    const auto a = dcmp('a');
    std::cout << path_comp::make_html_path(a, "container") << std::endl;

    std::cout << a.loops()[0].max()[0] << " " << a.loops()[0].max()[1] << std::endl;
    std::cout << a.loops()[0].min()[0] << " " << a.loops()[0].min()[1] << std::endl;

    std::cout << path_comp::make_html_path(dcmp('B'), "container2") << std::endl;

    ftdcmp::release();

    return 0;
}
