project(path_comp)

add_library(path_comp INTERFACE
    src/interface.hpp
    src/impl.hpp
)

target_include_directories(path_comp INTERFACE src)
