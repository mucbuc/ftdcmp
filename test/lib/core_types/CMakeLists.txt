project(core_types)

add_library(core_types INTERFACE 
  src/vec2.hpp
  src/vec3.hpp
  src/rect2.hpp
  src/rect3.hpp
  src/scalars.hpp
)

set_target_properties(core_types PROPERTIES
  CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	COMPILE_WARNING_AS_ERROR ON
    LINKER_LANGUAGE CXX
)

