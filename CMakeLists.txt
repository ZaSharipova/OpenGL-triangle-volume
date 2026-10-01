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
set(GEOMETRY_SOURCES
    ${SRC_DIR}/geometry/geometry_subsidiary.cpp
    ${SRC_DIR}/geometry/intersection.cpp
    ${SRC_DIR}/geometry/triangle.cpp
    ${SRC_DIR}/geometry/vec3.cpp
    ${SRC_DIR}/geometry/triangle_set.cpp)


set(TEST_DIR ${SRC_DIR}/tests)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR} ${INCLUDE_DIR}/geometry)

add_library(geometry ${GEOMETRY_SOURCES})
add_library(subsidiary ${SRC_DIR}/subsidiary.cpp)
add_library(interface ${SRC_DIR}/gl_process.cpp ${SRC_DIR}/input.cpp src/glad.c)
target_link_libraries(interface PUBLIC glfw OpenGL::GL)

add_executable(gl_3d ${SRC_DIR}/main.cpp)
target_link_libraries(gl_3d PRIVATE geometry subsidiary interface)

add_executable(tests ${SRC_DIR}/test_have_intersection.cpp)
target_link_libraries(tests PRIVATE geometry GTest::gtest_main)

add_executable(triangle_set_test ${SRC_DIR}/triangle_set_test.cpp)
target_link_libraries(triangle_set_test PRIVATE geometry GTest::gtest_main)

add_executable(test_end_to_end ${SRC_DIR}/test_end_to_end.cpp)
target_link_libraries(test_end_to_end PRIVATE geometry subsidiary interface GTest::gtest_main)
