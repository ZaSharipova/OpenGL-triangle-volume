#ifndef SHADER_PROGRAM_HPP_
#define SHADER_PROGRAM_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

constexpr size_t LOG_ARRAY_SIZE = 512;

unsigned int CompileShader(GLenum type, const char* src);
unsigned int LinkProgram(unsigned int vertex_shader, unsigned int fragment_shader);
unsigned int CreateShaderProgram(const char* vertex_src, const char* fragment_src);

#endif // SHADER_PROGRAM_HPP_
