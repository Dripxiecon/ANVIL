cmake_minimum_required(VERSION 3.22)
project(anvil_mesh_tests LANGUAGES CXX)

enable_testing()

set(ANVIL_MESH_SOURCES
    ../../../../../engine/geometry/mesh/mesh_data.cpp
    ../../../../../engine/geometry/mesh/operations/edge_split.cpp
    ../../../../../engine/geometry/mesh/operations/face_extrude.cpp
    ../../../../../engine/geometry/mesh/operations/face_inset.cpp
)

function(add_anvil_mesh_test target source)
    add_executable(${target} ${source} ${ANVIL_MESH_SOURCES})
    target_compile_features(${target} PRIVATE cxx_std_20)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    add_test(NAME ${target} COMMAND ${target})
endfunction()

add_anvil_mesh_test(test_mesh_data test_mesh_data.cpp)
add_anvil_mesh_test(test_mesh_editing test_mesh_editing.cpp)
