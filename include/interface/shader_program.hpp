#ifndef SHADER_PROGRAM_HPP_
#define SHADER_PROGRAM_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Interface {

constexpr size_t LOG_ARRAY_SIZE = 512;

unsigned int CompileShader(GLenum type, const char* src);
unsigned int LinkProgram(unsigned int vertex_shader, unsigned int fragment_shader);

class ShaderProgram {
public:
    ShaderProgram(const char* vertex_src, const char* fragment_src);

    ~ShaderProgram() {
        if (program_ != 0) {
            glDeleteProgram(program_);
        }
    }

    ShaderProgram(ShaderProgram&& other) noexcept : program_(other.program_) {
        other.program_ = 0;
    }

    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    unsigned int Get() const {
        return program_;
    }

private:
    unsigned int program_ = 0;
};

} // namespace Interface

#endif // SHADER_PROGRAM_HPP_
