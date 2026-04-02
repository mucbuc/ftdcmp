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


if(PROJECT_IS_TOP_LEVEL) # or libs??? 
	include_directories(PUBLIC SYSTEM test/lib/FreeType/include)
    include_directories(PUBLIC SYSTEM test/lib)
else()
    include_directories(PUBLIC SYSTEM ../FreeType/include)
	include_directories(PUBLIC SYSTEM ..)
endif()