#include "shader_program.hpp"

#include <cassert>
#include <vector>
#include <iostream>

unsigned int CompileShader(GLenum type, const char* src) {
    assert(src);

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success_flag = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success_flag);
    if (!success_flag) {
        char* log = new char [LOG_ARRAY_SIZE];

        glGetShaderInfoLog(shader, LOG_ARRAY_SIZE, nullptr, log);
        std::cerr << "Shader error: " << log << "\n";
        delete[] log;
    }

    return shader;
}

unsigned int LinkProgram(unsigned int vertex_shader, unsigned int fragment_shader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int linked_success_flag = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked_success_flag);
    if (!linked_success_flag) {
        char* log = new char[LOG_ARRAY_SIZE];
        glGetProgramInfoLog(program, LOG_ARRAY_SIZE, nullptr, log);
        std::cerr << "Link error: " << log << "\n";
        delete[] log;
    }

    return program;
}

unsigned int CreateShaderProgram(const char* vertex_src, const char* fragment_src) {
    unsigned int vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_src);
    unsigned int fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_src);

    unsigned int program = LinkProgram(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}
