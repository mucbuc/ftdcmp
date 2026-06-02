cmake_minimum_required(VERSION 3.27)
project(ftdcmp)

add_subdirectory(lib/freetype)

add_library(ftdcmp STATIC
    ftdcmp.cpp
    ftdcmp.hpp
)

set_target_properties(ftdcmp PROPERTIES
    CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	COMPILE_WARNING_AS_ERROR ON
)

target_include_directories(ftdcmp SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/lib/freetype/include)


