project(path_comp)

add_library(path_comp INTERFACE
    src/loop.hpp
    src/loop_impl.hpp
)

set_target_properties(path_comp PROPERTIES
 	CXX_STANDARD 23
 	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	COMPILE_WARNING_AS_ERROR ON
)

target_include_directories(path_comp INTERFACE src)
