#include "shader_program.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace Interface {

namespace {

template <typename GetParamFn, typename GetLogFn>
std::string GetInfoLog(GLuint object, GetParamFn get_param, GetLogFn get_log) {
    GLint length = 0;
    get_param(object, GL_INFO_LOG_LENGTH, &length);

    if (length <= 1) {
        return {};
    }

    std::string log(static_cast<std::size_t>(length), '\0');
    GLsizei written = 0;
    get_log(object, length, &written, log.data());
    log.resize(static_cast<std::size_t>(written));
    return log;
}

std::string GetShaderInfoLog(GLuint shader) {
    return GetInfoLog(shader, glGetShaderiv, glGetShaderInfoLog);
}

std::string GetProgramInfoLog(GLuint program) {
    return GetInfoLog(program, glGetProgramiv, glGetProgramInfoLog);
}

} // namespace


unsigned int CompileShader(GLenum type, const char* src) {
    assert(src);

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success_flag = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success_flag);
    if (!success_flag) {
        std::string log = GetShaderInfoLog(shader);
        glDeleteShader(shader);
        std::cerr << "Shader error: " << log << "\n";
        throw std::runtime_error("Shader compilation failed: " + log);
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
        std::string log = GetProgramInfoLog(program);
        glDeleteProgram(program);
        std::cerr << "Link error: " << log << "\n";
        throw std::runtime_error("Shader linking failed: " + log);
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
