# ./blocks/basic/CMakeLists.txt

# list module block headers (absolute or relative)
set(MY_HEADERS
        include/gnuradio-4.x/basic/block0.hpp
        include/gnuradio-4.x/basic/block1.hpp
        include/gnuradio-4.x/basic/block2.hpp
        include/gnuradio-4.x/basic/block3.hpp
        include/gnuradio-4.x/basic/blockN.hpp
        # add more as necessary
)

# if there are additional C++ sources required for the build e.g. in blocks/basic/src
file(GLOB MY_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

create_block_library(grBasic
        HEADERS ${MY_HEADERS}
        SOURCES ${MY_SRCS}
)

# optionally:
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include/gnuradio-4.x/basic FILES_MATCHING PATTERN "*.hpp")

if(ENABLE_TESTING)
    add_subdirectory(test)
endif()
