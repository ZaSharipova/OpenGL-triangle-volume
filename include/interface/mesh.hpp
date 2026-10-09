#ifndef MESH_HPP_
#define MESH_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

struct VertexAttribute {
    GLuint location;
    GLint componentCount;
    GLsizei strideFloats;
    size_t offsetFloats;
};

struct Mesh {
    unsigned int vao = 0;
    unsigned int vbo = 0;
    GLsizei vertexCount = 0;
};

Mesh CreateMesh(const std::vector<float>& data, const std::vector<VertexAttribute>& attributes,
                GLsizei verticesPerElement);
void DestroyMesh(const Mesh& mesh);
#endif // MESH_HPP_
