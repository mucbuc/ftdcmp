cmake_minimum_required(VERSION 3.27)

project(Example)

add_executable(Example src/check_assert.cpp)

set_target_properties(Example PROPERTIES
    CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	COMPILE_WARNING_AS_ERROR ON
)

target_link_libraries(Example INTERFACE asserter)
#add_definitions(-DNDEBUG)
