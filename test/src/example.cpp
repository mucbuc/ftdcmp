#include <ftdcmp/ftdcmp.hpp>
#include <iostream>

#include <asserter/src/asserter.hpp>
#include <path_comp/src/make_html_path.hpp>

int main()
{
    ftdcmp::init();
    {
#ifdef __EMSCRIPTEN__
        auto dcmp = ftdcmp::make_decomposer_f("ArialUnicode.ttf");
#else
        auto dcmp = ftdcmp::make_decomposer_f("/Library/Fonts/Arial Unicode.ttf");
#endif

        const auto a = dcmp('a');
        std::cout << path_comp::make_html_path(a, "container") << std::endl;

        std::cout << a.bounds_end_points() << std::endl;

        std::cout << path_comp::make_html_path(dcmp('B'), "container2") << std::endl;
    }

    ftdcmp::release();

    return 0;
}
