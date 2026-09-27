cmake_minimum_required(VERSION 3.15)
project(gl_3d)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

find_package(GTest REQUIRED)
enable_testing()
include(GoogleTest)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(triangle triangle.cpp)

add_executable(gl_3d main.cpp subsidiary.cpp src/glad.c)
target_include_directories(gl_3d PRIVATE include)
target_link_libraries(gl_3d PRIVATE triangle glfw OpenGL::GL)

add_executable(tests test_have_intersection.cpp)
target_link_libraries(tests PRIVATE triangle GTest::gtest_main)
