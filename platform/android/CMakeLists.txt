cmake_minimum_required(VERSION 3.22.1)
project(anvil_android LANGUAGES CXX)

add_library(
    anvil_android
    SHARED
    native_bridge.cpp
)

target_compile_features(anvil_android PRIVATE cxx_std_20)

target_compile_options(
    anvil_android
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(
    anvil_android
    PRIVATE
    GLESv3
    log
)
