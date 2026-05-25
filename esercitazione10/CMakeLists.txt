cmake_minimum_required(VERSION 3.20)
project(esercitazione10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla -Wno-sign-compare")

enable_testing()

add_executable(test_gradiente_coniugato test_gradient.cpp)
target_include_directories(test_gradiente_coniugato PRIVATE /usr/include/eigen3)
add_test(NAME gradiente_coniugato COMMAND test_gradiente_coniugato)

