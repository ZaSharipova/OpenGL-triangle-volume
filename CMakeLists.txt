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
set(GEOMETRY_DIR ${SRC_DIR}/geometry)
set(GEOMETRY_SOURCES
    ${GEOMETRY_DIR}/geometry_subsidiary.cpp
    ${GEOMETRY_DIR}/intersection.cpp
    ${GEOMETRY_DIR}/triangle.cpp
    ${GEOMETRY_DIR}/vec3.cpp
    ${GEOMETRY_DIR}/triangle_set.cpp)

set(INTERFACE_DIR ${SRC_DIR}/interface)
set(INTERFACE_SOURCES
    ${INTERFACE_DIR}/input.cpp
    ${INTERFACE_DIR}/input_controller.cpp
    ${INTERFACE_DIR}/mesh.cpp
    ${INTERFACE_DIR}/renderer.cpp
    ${INTERFACE_DIR}/shader_program.cpp
    ${INTERFACE_DIR}/window.cpp)


set(TEST_DIR ${SRC_DIR}/tests)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR} ${INCLUDE_DIR}/geometry)

add_library(geometry ${GEOMETRY_SOURCES})
add_library(interface ${INTERFACE_SOURCES} src/glad.c)
target_include_directories(interface PUBLIC ${INCLUDE_DIR}/interface)
add_library(subsidiary ${SRC_DIR}/subsidiary.cpp)
target_link_libraries(interface PUBLIC glfw OpenGL::GL)

add_executable(gl_3d ${SRC_DIR}/main.cpp)
target_link_libraries(gl_3d PRIVATE geometry subsidiary interface)

add_executable(test_intersection ${SRC_DIR}/test_have_intersection.cpp)
target_link_libraries(test_intersection PRIVATE geometry GTest::gtest_main)

add_executable(triangle_set_test ${SRC_DIR}/triangle_set_test.cpp)
target_link_libraries(triangle_set_test PRIVATE geometry GTest::gtest_main)

add_executable(test_end_to_end ${SRC_DIR}/test_end_to_end.cpp)
target_link_libraries(test_end_to_end PRIVATE geometry subsidiary interface GTest::gtest_main)
