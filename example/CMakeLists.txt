# example/CMakeLists.txt

add_library(gr_common INTERFACE)

add_executable(test_registry_basic main.cpp)
add_executable(test_registry_math main.cpp)
add_executable(test_registry_both main.cpp)
add_executable(test_registry main.cpp)

target_link_libraries(test_registry_basic PRIVATE gr_common grCore grBasic BlockLib::Registry)
target_link_libraries(test_registry_math PRIVATE gr_common grCore grMath grBasic BlockLib::Registry)
target_link_libraries(test_registry_both PRIVATE gr_common grCore grBasic grMath BlockLib::Registry)
target_link_libraries(test_registry PRIVATE gr_common grCore grBlockLib BlockLib::Registry)

install(TARGETS test_registry RUNTIME DESTINATION bin)
