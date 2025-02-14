# blocks/math/CMakeLists.txt

# list your headers (absolute or relative)
set(MATH_HEADERS
        include/gnuradio-4.x/math/math_block0.hpp
        # add more as necessary
)

# if there are C++ sources in e.g. blocks/math/src
file(GLOB MY_MATH_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

create_block_library(grMath
        HEADERS ${MATH_HEADERS}
        SOURCES ${MY_MATH_SRCS}
)

# optionally:
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include/gnuradio-4.x/math FILES_MATCHING PATTERN "*.hpp")

if(ENABLE_TESTING)
    add_subdirectory(test)
endif()

