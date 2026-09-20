cmake_minimum_required(VERSION 3.15)
project(gl_3d)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(gl_3d main.cpp triangle.cpp src/glad.c)
target_include_directories(gl_3d PRIVATE include)
target_link_libraries(gl_3d PRIVATE glfw OpenGL::GL)
