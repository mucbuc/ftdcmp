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
    target_link_options(ftdcmp PRIVATE 
        -USE_FREETYPE=1
    )
else()
    target_link_libraries(ftdcmp PRIVATE freetype)
endif()


