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

if (NOT EMSCRIPTEN)
    add_subdirectory(lib/FreeType EXCLUDE_FROM_ALL)
    target_link_libraries(ftdcmp PRIVATE freetype)
endif()

target_include_directories(ftdcmp SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/lib/FreeType/include)