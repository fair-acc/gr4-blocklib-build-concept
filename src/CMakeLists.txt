# src/CMakeLists.txt

add_library(grCore STATIC
        gr_registry.cpp
        # add other .cpp if needed
)
target_include_directories(grCore PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
