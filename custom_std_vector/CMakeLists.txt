cmake_minimum_required(VERSION 3.20)

project(custom_vector LANGUAGES CXX)

add_executable(vector_tests tests/VectorTests.cpp)
target_compile_features(vector_tests PRIVATE cxx_std_20)
target_include_directories(vector_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(vector_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(vector_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME vector_tests COMMAND vector_tests)
