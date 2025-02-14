cmake_minimum_required(VERSION 3.30)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

option(ENABLE_SPLIT "Enable splitting of generated files (pass --split to the parser)" ON)


# --------------------------------------------------------------------------
# Manually configure and build the parser tool in a separate directory
# --------------------------------------------------------------------------
set(TOOLS_BUILD_DIR "${CMAKE_BINARY_DIR}/tools_build")
if(NOT EXISTS "${TOOLS_BUILD_DIR}/CMakeCache.txt")
    message(STATUS "Configuring parser tool in ${TOOLS_BUILD_DIR}...")
    execute_process(
            COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR}/tools -B ${TOOLS_BUILD_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            RESULT_VARIABLE config_result
            OUTPUT_VARIABLE config_output
            ERROR_VARIABLE config_error
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE
    )
    if(NOT config_result EQUAL 0)
        message(FATAL_ERROR "Failed to configure parser tool: ${config_error}")
    endif()
endif()

message(STATUS "Building parser tool in ${TOOLS_BUILD_DIR}...")
execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${TOOLS_BUILD_DIR} --target parse_registrations
        WORKING_DIRECTORY ${TOOLS_BUILD_DIR}
        RESULT_VARIABLE build_result
        OUTPUT_VARIABLE build_output
        ERROR_VARIABLE build_error
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
)
if(NOT build_result EQUAL 0)
    message(FATAL_ERROR "Failed to build parse_registrations tool: ${build_error}")
else ()
    set(PARSER_EXECUTABLE "${TOOLS_BUILD_DIR}/parse_registrations")
    message(STATUS "Parser tool built: ${PARSER_EXECUTABLE}")
endif()

# --------------------------------------------------------------------------
# Configure the rest of the project
# --------------------------------------------------------------------------

# add core functionality such as registry
add_subdirectory(include)
add_subdirectory(src)

# define global block building command
function(create_block_library LIB_NAME)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs HEADERS SOURCES)
    cmake_parse_arguments(CBL "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # directory where parse_registrations will output .cpp files.
    set(GEN_DIR "${CMAKE_BINARY_DIR}/generated_plugins/${LIB_NAME}")
    file(MAKE_DIRECTORY "${GEN_DIR}")

    set(GEN_CPP_LIST "")
    foreach(HDR IN LISTS CBL_HEADERS)
        get_filename_component(ABS_HDR "${HDR}" ABSOLUTE)
        get_filename_component(BASENAME "${HDR}" NAME_WE)

        # remove any previously generated .cpp files for this header.
        file(GLOB OLD_FILES "${GEN_DIR}/${BASENAME}*.cpp")
        if(OLD_FILES)
            message(STATUS "deleting old generated files ${OLD_FILES}")
            file(REMOVE ${OLD_FILES})
        endif()

        if(ENABLE_SPLIT)
            set(PARSER_SPLIT_OPTION "--split")
        else()
            set(PARSER_SPLIT_OPTION "")
        endif()

        message(STATUS "Generating code for header: ${ABS_HDR} -> ${GEN_DIR} with PARSER_SPLIT_OPTION=${PARSER_SPLIT_OPTION}")

        # run the parser tool immediately at configuration time.
        execute_process(
                COMMAND ${PARSER_EXECUTABLE} "${ABS_HDR}" "${GEN_DIR}" ${PARSER_SPLIT_OPTION}
                RESULT_VARIABLE gen_res
                OUTPUT_VARIABLE gen_out
                ERROR_VARIABLE gen_err
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "Output from parse_registrations for ${ABS_HDR}:\n${gen_out}")
        if(NOT gen_res EQUAL 0)
            message(FATAL_ERROR "Error running parse_registrations on ${HDR}: ${gen_err}")
        endif()

        # Glob for the generated .cpp files.
        file(GLOB GENERATED_FILES "${GEN_DIR}/${BASENAME}*.cpp")
        if(NOT GENERATED_FILES)
            # If no .cpp files were generated, create a dummy file.
            set(DUMMY_CPP "${GEN_DIR}/dummy_${BASENAME}.cpp")
            file(WRITE "${DUMMY_CPP}" "// No macros or expansions found for '${BASENAME}'\n")
            list(APPEND GENERATED_FILES "${DUMMY_CPP}")
        endif()

        list(APPEND GEN_CPP_LIST ${GENERATED_FILES})
    endforeach()

    add_library(${LIB_NAME} SHARED
            ${GEN_CPP_LIST}
            ${CBL_SOURCES}
    )
    target_include_directories(${LIB_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:include>
            "${CMAKE_SOURCE_DIR}/include"
    )
    set_target_properties(${LIB_NAME} PROPERTIES CXX_STANDARD 23)
    install(TARGETS ${LIB_NAME} LIBRARY DESTINATION lib)
endfunction()

# --------------------------------------------------------------------------
# 4. Add subdirectories defining libraries via create_block_library
# --------------------------------------------------------------------------
add_subdirectory(blocks)

# Optionally build aggregator library.
if (TARGET grBasic AND TARGET grMath)
    add_library(grBlockLib SHARED src/dummy.cpp)
    target_link_libraries(grBlockLib PUBLIC grBasic grMath)
    set_target_properties(grBlockLib PROPERTIES CXX_STANDARD 23)
    install(TARGETS grBlockLib LIBRARY DESTINATION lib)
endif()

add_subdirectory(example)
