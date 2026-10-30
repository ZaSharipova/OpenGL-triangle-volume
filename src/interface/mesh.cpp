#include "mesh.hpp"

Mesh::Mesh(const std::vector<float>& data, const std::vector<VertexAttribute>& attributes,
           GLsizei verticesPerElement) : vertexCount_(static_cast<GLsizei>(data.size()) / verticesPerElement) {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), GL_STATIC_DRAW);

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
