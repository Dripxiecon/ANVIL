cmake_minimum_required(VERSION 3.22)
project(anvil_core_checks LANGUAGES CXX)

enable_testing()

add_subdirectory(tests/unit/core/math)
add_subdirectory(tests/unit/core/scene)
add_subdirectory(tests/unit/engine/geometry/mesh)
