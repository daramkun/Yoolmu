cmake_minimum_required(VERSION 4.0)

include(${CMAKE_CURRENT_LIST_DIR}/Platform.cmake)

function(YoolmuProject name kind)
    string(REPLACE "::" "_" INTERNAL_NAME "${name}")
    string(TOUPPER "${INTERNAL_NAME}" INTERNAL_NAME)

    if("${kind}" STREQUAL "exe")
        add_executable(${INTERNAL_NAME} WIN32)
        add_executable(${name} ALIAS ${INTERNAL_NAME})
    elseif("${kind}" STREQUAL "dll")
        add_library(${INTERNAL_NAME} SHARED)
        add_library(${name} ALIAS ${INTERNAL_NAME})
        if(${PLATFORM_WINDOWS})
            target_compile_definitions(${INTERNAL_NAME} INTERFACE "${INTERNAL_NAME}_API=__declspec(dllimport)")
            target_compile_definitions(${INTERNAL_NAME} PRIVATE   "${INTERNAL_NAME}_API=__declspec(dllexport)")
        else()
            target_compile_definitions(${INTERNAL_NAME} INTERFACE "${INTERNAL_NAME}_API")
        endif()

    elseif("${kind}" STREQUAL "lib")
        add_library(${INTERNAL_NAME} STATIC)
        add_library(${name} ALIAS ${INTERNAL_NAME})
    endif()

    target_include_directories(${INTERNAL_NAME} PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public/> $<INSTALL_INTERFACE:${INSTALL_INC_DIR}>)
    target_include_directories(${INTERNAL_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private/")

    set_target_properties(${INTERNAL_NAME} PROPERTIES
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON
    )

    if(${PLATFORM_WINDOWS})
        target_compile_definitions(${INTERNAL_NAME} PRIVATE WINDOWS _WINDOWS UNICODE _UNICODE WIN32 _WIN32 WIN64 _WIN64)
    endif()

    if(${COMPILER_MSVC})
        target_compile_options(${INTERNAL_NAME} PRIVATE /utf-8)
    endif()

    file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/" "*.h" "*.hpp" "*.c" "*.cpp" "*.cxx")
    file(GLOB_RECURSE MODULES "${CMAKE_CURRENT_SOURCE_DIR}/" "*.cppm")
    target_sources(${INTERNAL_NAME} PRIVATE ${SOURCES})
    target_sources(${INTERNAL_NAME} PRIVATE FILE_SET CXX_MODULES FILES ${MODULES})
endfunction()

function(YoolmuProjectReference name project)
    string(REPLACE "::" "_" INTERNAL_NAME "${name}")
    string(TOUPPER "${INTERNAL_NAME}" INTERNAL_NAME)

    add_dependencies(${INTERNAL_NAME} ${project})
    target_link_libraries(${INTERNAL_NAME} PUBLIC ${project})
endfunction()

function(YoolmuProjectAddPackage name package)
    string(REPLACE "::" "_" INTERNAL_NAME "${name}")
    string(TOUPPER "${INTERNAL_NAME}" INTERNAL_NAME)

    find_package(${package})

    if(${package}_FOUND)
        target_include_directories(${INTERNAL_NAME} PUBLIC ${package}_INCLUDE_DIR ${package}_INCLUDE_DIRS)
        target_link_libraries(${INTERNAL_NAME} PUBLIC ${package}_LIBRARY ${package}_LIBRARIES)
    endif()
endfunction()
