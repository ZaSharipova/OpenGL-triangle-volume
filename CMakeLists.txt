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

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${SRC_DIR}/tests)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

add_library(triangle ${SRC_DIR}/triangle.cpp)
add_library(subsidiary ${SRC_DIR}/subsidiary.cpp)
add_library(interface ${SRC_DIR}/gl_process.cpp ${SRC_DIR}/input.cpp src/glad.c)
target_link_libraries(interface PUBLIC glfw OpenGL::GL)

add_executable(gl_3d ${SRC_DIR}/main.cpp)
# target_include_directories(gl_3d PRIVATE ${INCLUDE_DIR})
target_link_libraries(gl_3d PRIVATE triangle subsidiary interface)

add_executable(tests ${SRC_DIR}/test_have_intersection.cpp)
target_link_libraries(tests PRIVATE triangle GTest::gtest_main)
