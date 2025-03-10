# include/CMakeLists.txt

add_library(Registry INTERFACE)
add_library(BlockLib::Registry ALIAS Registry)

target_include_directories(Registry INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/BlockLib>
)

install(TARGETS Registry EXPORT BlockLibTargets)
install(FILES gr_registry.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/BlockLib)
