#include "mesh.hpp"

Mesh CreateMesh(const std::vector<float>& data, const std::vector<VertexAttribute>& attributes,
                GLsizei verticesPerElement) {
    Mesh mesh;
    mesh.vertexCount = static_cast<GLsizei>(data.size()) / verticesPerElement;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
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

    return mesh;
}

void DestroyMesh(const Mesh& mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
}
