cmake_minimum_required(VERSION 3.22)
project(anvil_math_tests LANGUAGES CXX)

enable_testing()

function(add_anvil_math_test target source)
    add_executable(${target} ${source})
    target_compile_features(${target} PRIVATE cxx_std_20)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    add_test(NAME ${target} COMMAND ${target})
endfunction()

add_anvil_math_test(test_vector3 test_vector3.cpp)
add_anvil_math_test(test_quaternion test_quaternion.cpp)
add_anvil_math_test(test_transform test_transform.cpp)
add_anvil_math_test(test_matrix4 test_matrix4.cpp)
