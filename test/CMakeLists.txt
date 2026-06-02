cmake_minimum_required(VERSION 3.27)

project(Example)

add_executable(Example src/example.cpp)
set_target_properties(Example PROPERTIES
    CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	COMPILE_WARNING_AS_ERROR ON
)

add_subdirectory(lib/ftdcmp)
target_include_directories(ftdcmp SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/lib)
target_include_directories(ftdcmp SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/lib/freetype/include)

add_subdirectory(lib/path_comp)
add_subdirectory(lib/asserter)
add_subdirectory(lib/core_types)

add_subdirectory(lib/freetype)

target_link_libraries(Example PRIVATE ftdcmp freetype)

#include_directories(PUBLIC SYSTEM ${CMAKE_CURRENT_LIST_DIR}/lib)
