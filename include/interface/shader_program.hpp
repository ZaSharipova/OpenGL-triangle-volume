#ifndef SHADER_PROGRAM_HPP_
#define SHADER_PROGRAM_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <string>

namespace Interface {

namespace detail {

template <typename GetParamFn, typename GetLogFn>
std::string GetInfoLog(GLuint object, GetParamFn get_param, GetLogFn get_log) {
    GLint length = 0;
    get_param(object, GL_INFO_LOG_LENGTH, &length);

    if (length <= 1) {
        return { };
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

} // namespace detail

// unsigned int CompileShader(GLenum type, const char* src);
unsigned int LinkProgram(unsigned int vertex_shader, unsigned int fragment_shader);

class Shader {
public:
    Shader(GLenum type, const char* src) {
        assert(src);

        shader_ = glCreateShader(type);
        glShaderSource(shader_, 1, &src, nullptr);
        glCompileShader(shader_);

        int success_flag = 0;
        glGetShaderiv(shader_, GL_COMPILE_STATUS, &success_flag);
        if (!success_flag) {
            std::string log = detail::GetShaderInfoLog(shader_);
            glDeleteShader(shader_);
            std::cerr << "Shader error: " << log << "\n";
            throw std::runtime_error("Shader compilation failed: " + log);
        }
    }

    ~Shader() {
        if (shader_ != 0) {
            glDeleteShader(shader_);
        }
    }

    unsigned int Get() const {
        return shader_;
    }

private:
    unsigned int shader_ = 0;
};

class ShaderProgram {
public:
    ShaderProgram(const char* vertex_src, const char* fragment_src) {
        Shader vertex_shader(GL_VERTEX_SHADER, vertex_src);
        Shader fragment_shader(GL_FRAGMENT_SHADER, fragment_src);
        LinkProgram(vertex_shader, fragment_shader);
    }

    ~ShaderProgram() {
        if (program_ != 0) {
            glDeleteProgram(program_);
        }
    }

    ShaderProgram(ShaderProgram&& other) noexcept
        : program_(other.program_) {
        other.program_ = 0;
    }

    ShaderProgram& operator=(ShaderProgram&& other) noexcept {
        if (this != &other) {
            if (program_ != 0) {
                glDeleteProgram(program_);
            }

            program_ = other.program_;
            other.program_ = 0;
        }

        return *this;
    }

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    unsigned int Get() const {
        return program_;
    }

private:
    void LinkProgram(const Shader& vertex_shader, const Shader& fragment_shader) {
        program_ = glCreateProgram();
        glAttachShader(program_, vertex_shader.Get());
        glAttachShader(program_, fragment_shader.Get());
        glLinkProgram(program_);

        int linked_success_flag = 0;
        glGetProgramiv(program_, GL_LINK_STATUS, &linked_success_flag);
        if (!linked_success_flag) {
            std::string log = detail::GetProgramInfoLog(program_);
            glDeleteProgram(program_);
            std::cerr << "Link error: " << log << "\n";
            throw std::runtime_error("Shader linking failed: " + log);
        }
    }

    unsigned int program_ = 0;
};

} // namespace Interface

#endif // SHADER_PROGRAM_HPP_
