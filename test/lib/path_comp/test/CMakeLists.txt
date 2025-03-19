cmake_minimum_required(VERSION 3.27)

project(path_comp_sample)

add_executable(path_comp_sample
    src/example.cpp
)
set_target_properties(path_comp_sample PROPERTIES
    CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	COMPILE_WARNING_AS_ERROR ON
)

add_subdirectory(lib/path_comp)
add_subdirectory(lib/asserter)

if (EMSCRIPTEN)
    set_target_properties(path_comp_sample PROPERTIES SUFFIX ".html")
    target_link_options(path_comp_sample PRIVATE "-sUSE_WEBGPU=1" "-sUSE_GLFW=3")
endif()

target_include_directories(path_comp_sample SYSTEM PUBLIC lib)
