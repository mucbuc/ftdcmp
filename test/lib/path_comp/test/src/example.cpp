#include <iostream>
#include <sstream>

#include <array>
#include <asserter/src/test.hpp>
#include <path_comp/src/interface.hpp>

using namespace std;

using vector_type = std::array<float, 2>;
using loop_type = path_comp::Loop<vector_type>;
using comp_type = path_comp::Comp<loop_type>;

static void test_make_html_path()
{
    auto b = loop_type({ 30, 50 })
            .line({ 10, 60 });

    auto c = comp_type({ 70, 80 }).insert(b);

    std::cout << make_html_path(c) << std::endl;
}

static void test_convert()
{
    auto b = loop_type({ 30, 50 })
                 .line({ 10, 60 });

    const auto l = b.convert_to<std::array<long, 2>>();

    auto c = comp_type({ 70, 80 }).insert(b);
    const auto ll = c.convert_to<std::array<long, 2>>();
}

static void test_close()
{
    // make sure that close is a no op

    auto b = loop_type({ 30, 50 })
                 .line({ 10, 60 });

    ASSERT(b.points().size() == 2);
    ASSERT(b.segments().size() == 1);
    b.close();

    ASSERT(b.points().size() == 2);
    ASSERT(b.segments().size() == 2);

    auto c = loop_type({ 30, 50 })
                 .line({ 10, 60 })
                 .line({ 30, 50 });

    ASSERT(c.points().size() == 3);
    ASSERT(c.segments().size() == 2);
    c.close();

    ASSERT(c.points().size() == 2)
    (c.points().size());
    ASSERT(c.segments().size() == 2);

    FOOTER;
}

static void test_comp()
{
    auto b = loop_type({ 30, 50 })
                 .line({ 10, 60 })
                 .curve({ 50, 20 }, { 100, 100 })
                 .line({ 100, 100 })
                 .line({ 10, 30 });

    auto c = comp_type({ 70, 80 }).insert(b);

    ASSERT(c.size() == vector_type({ 70, 80 }));
    ASSERT(c.loops().size() == 1);
    ASSERT(c.loops()[0].points().size() == 6);

    FOOTER;
}

static void test_points_and_segments()
{
    auto b = loop_type({ 30, 50 })
                 .line({ 10, 60 })
                 .curve({ 50, 20 }, { 100, 100 })
                 .curve({ 50, 20 }, { 100, 100 }, { 100, 100 })
                 .line({ 10, 30 });

    ASSERT(b.points().size() == 8);
    ASSERT(b.segments().size() == 4)
    (b.segments().size());

    auto c = loop_type({ 1, 1 });

    ASSERT(c.points().size() == 1);
    ASSERT(c.segments().size() == 0);

    FOOTER;
}

int main()
{
    test_make_html_path(); 
    test_convert();
    test_close();
    test_comp();
    test_points_and_segments();
    return 0;
}
