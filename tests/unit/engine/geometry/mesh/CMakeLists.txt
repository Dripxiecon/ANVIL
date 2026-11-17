cmake_minimum_required(VERSION 3.22)
project(anvil_mesh_tests LANGUAGES CXX)

enable_testing()

add_executable(
    test_mesh_data
    test_mesh_data.cpp
    ../../../../../engine/geometry/mesh/mesh_data.cpp
)

target_compile_features(test_mesh_data PRIVATE cxx_std_20)
target_compile_options(test_mesh_data PRIVATE -Wall -Wextra -Wpedantic)
add_test(NAME test_mesh_data COMMAND test_mesh_data)
