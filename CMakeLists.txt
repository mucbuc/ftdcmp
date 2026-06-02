cmake_minimum_required(VERSION 3.27)
project(ftdcmp)

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

if (EMSCRIPTEN)
    target_include_directories(ftdcmp SYSTEM PRIVATE ${EMSCRIPTEN_SYSROOT}/include/freetype2)
else()
    add_subdirectory(lib/freetype)
    target_include_directories(ftdcmp SYSTEM PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lib/freetype/include)
endif()




