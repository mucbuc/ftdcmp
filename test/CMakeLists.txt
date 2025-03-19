cmake_minimum_required(VERSION 3.27)

project(Example)

add_executable(Example src/example.cpp)
set_target_properties(Example PROPERTIES
    CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	COMPILE_WARNING_AS_ERROR ON
)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
add_compile_definitions(TARGET_HEADLESS)

add_subdirectory(lib/ftdcmp)
target_link_libraries(Example PRIVATE ftdcmp)

add_subdirectory(lib/path_comp)

add_subdirectory(lib/asserter)

if (EMSCRIPTEN)
    set_target_properties(Example PROPERTIES SUFFIX ".html")
    target_link_options(Example PRIVATE "-sUSE_FREETYPE=1" "-sALLOW_MEMORY_GROWTH" --preload-file /Library/Fonts/Arial\ Unicode.ttf@ArialUnicode.ttf)
else()
    add_subdirectory(lib/FreeType)
    target_link_libraries(Example PRIVATE freetype)
endif()

include_directories(PUBLIC SYSTEM ${CMAKE_SOURCE_DIR}/lib)
