#ifndef MESH_HPP_
#define MESH_HPP_

#include <glad/glad.h>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace Interface {

class VertexArray {
public:
    VertexArray() {
        glGenVertexArrays(1, &vao_);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            vao_ = 0;
            std::cerr << "glGenVertexArrays failed: " << error << std::endl;
            throw std::runtime_error("Failed to gen vertex array");
        }
    }

    ~VertexArray() {
        if (vao_ != 0) {
            glDeleteVertexArrays(1, &vao_);
        }
    }

    VertexArray(VertexArray&& other) noexcept : vao_(other.vao_) {
        other.vao_ = 0;
    }

    VertexArray& operator=(VertexArray&& other) noexcept {
        if (this != &other) {
            if (vao_ != 0) {
                glDeleteVertexArrays(1, &vao_);
            }

            vao_ = other.vao_;
            other.vao_ = 0;
        }

        return *this;
    }

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    unsigned int Get() const {
        return vao_;
    }

private:
    unsigned int vao_ = 0;
};

class VertexBuffer {
public:
    VertexBuffer() {
        glGenBuffers(1, &vbo_);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            vbo_ = 0;
            std::cerr << "glGenBuffers failed: " << error << std::endl;
            throw std::runtime_error("Failed to gen buffer");
        }
    }

    ~VertexBuffer() {
        if (vbo_ != 0) {
            glDeleteBuffers(1, &vbo_);
        }
    }

    VertexBuffer(VertexBuffer&& other) noexcept : vbo_(other.vbo_) {
        other.vbo_ = 0;
    }

    VertexBuffer& operator=(VertexBuffer&& other) noexcept {
        if (this != &other) {
            if (vbo_ != 0) {
                glDeleteBuffers(1, &vbo_);
            }

            vbo_ = other.vbo_;
            other.vbo_ = 0;
        }

        return *this;
    }

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    unsigned int Get() const {
        return vbo_;
    }

private:
    unsigned int vbo_ = 0;
};

struct VertexAttribute {
    GLuint location;
    GLint componentCount;
    GLsizei strideFloats;
    size_t offsetFloats;
};

class Mesh {
public:
    Mesh(const std::vector<float>& data, const std::vector<VertexAttribute>& attributes,
        GLsizei verticesPerElement);
    ~Mesh() = default;

    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    unsigned int GetVAO() const {
        return vao_.Get();
    }

    unsigned int GetVBO() const {
        return vbo_.Get();
    }

    GLsizei GetVertexCount() const {
        return vertexCount_;
    }

private:
    VertexArray vao_;
    VertexBuffer vbo_;
    GLsizei vertexCount_ = 0;
};

} // namespace Interface

#endif // MESH_HPP_
