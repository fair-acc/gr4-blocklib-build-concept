# example/CMakeLists.txt

add_library(gr_common INTERFACE)
target_include_directories(gr_common INTERFACE
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/blocks/basic/include"
        "${CMAKE_SOURCE_DIR}/blocks/math/include"
)

add_executable(test_registry_basic main.cpp)
add_executable(test_registry_math main.cpp)
add_executable(test_registry_both main.cpp)
add_executable(test_registry main.cpp)

target_link_libraries(test_registry_basic PRIVATE gr_common grCore grBasic)
target_link_libraries(test_registry_math PRIVATE gr_common grCore grMath)
target_link_libraries(test_registry_both PRIVATE gr_common grCore grBasic grMath)
target_link_libraries(test_registry PRIVATE gr_common grCore grBlockLib)

install(TARGETS test_registry RUNTIME DESTINATION bin)
