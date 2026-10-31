#include "shader_program.hpp"

#include <cassert>
#include <iostream>
#include <vector>

namespace Interface {

unsigned int CompileShader(GLenum type, const char* src) {
    assert(src);

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success_flag = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success_flag);
    if (!success_flag) {
        char log[LOG_ARRAY_SIZE]{};
        glGetShaderInfoLog(shader, LOG_ARRAY_SIZE, nullptr, log);
        std::cerr << "Shader error: " << log << "\n";
        glDeleteShader(shader);
        throw std::runtime_error(std::string("Shader compilation failed: ") + log);
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
        char log[LOG_ARRAY_SIZE]{};
        glGetProgramInfoLog(program, LOG_ARRAY_SIZE, nullptr, log);
        std::cerr << "Link error: " << log << "\n";
        glDeleteProgram(program);
        throw std::runtime_error(std::string("Shader linking failed: ") + log);
    }

    return program;
}

ShaderProgram::ShaderProgram(const char* vertex_src, const char* fragment_src) {
    unsigned int vertex_shader = 0;
    unsigned int fragment_shader = 0;

    try {
        vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_src);
        fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_src);
        program_ = LinkProgram(vertex_shader, fragment_shader);
    } catch (...) {
        if (fragment_shader != 0) {
            glDeleteShader(fragment_shader);
        }
        if (vertex_shader != 0) {
            glDeleteShader(vertex_shader);
        }
        throw;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
    if (this != &other) {
        if (program_ != 0) {
            glDeleteProgram(program_);
        }

        program_ = other.program_;
        other.program_ = 0;
    }

    return *this;
}

} // namespace Interface
