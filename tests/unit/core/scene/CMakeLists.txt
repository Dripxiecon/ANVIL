cmake_minimum_required(VERSION 3.22)
project(anvil_scene_tests LANGUAGES CXX)

enable_testing()

add_executable(
    test_scene
    test_scene.cpp
    ../../../../core/scene/scene.cpp
)

target_compile_features(test_scene PRIVATE cxx_std_20)
if(MSVC)
    target_compile_options(test_scene PRIVATE /W4 /permissive-)
else()
    target_compile_options(test_scene PRIVATE -Wall -Wextra -Wpedantic)
endif()
add_test(NAME test_scene COMMAND test_scene)
