#include "mesh.hpp"

#include <OpenGL/gl.h>
#include <iostream>

namespace Interface {

Mesh::Mesh(const std::vector<float>& data, const std::vector<VertexAttribute>& attributes,
    GLsizei verticesPerElement)
    : vertexCount_(static_cast<GLsizei>(data.size()) / verticesPerElement) {
    glGenVertexArrays(1, &vao_);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "glGenVertexArrays failed: " << error << std::endl;
        throw std::runtime_error("Failed to gen vertex array");
    }

    glGenBuffers(1, &vbo_);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "glGenBuffers failed: " << error << std::endl;
        throw std::runtime_error("Failed to gen buffer");
    }

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), GL_STATIC_DRAW);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "glBufferData failed: " << error << std::endl;
        throw std::runtime_error("Failed to connect buffer");
    }

    for (const VertexAttribute& attrib : attributes) {
        glVertexAttribPointer(
            attrib.location,
            attrib.componentCount,
            GL_FLOAT,
            GL_FALSE,
            attrib.strideFloats * static_cast<GLsizei>(sizeof(float)),
            reinterpret_cast<void*>(attrib.offsetFloats * sizeof(float)));

        glEnableVertexAttribArray(attrib.location);
    }
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

} // namespace Interface
