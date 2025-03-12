# src/CMakeLists.txt

add_library(grCore STATIC
        gr_registry.cpp
        # add other .cpp if needed
)

target_link_libraries(grCore PRIVATE BlockLib::Registry)
