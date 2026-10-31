#ifndef MESH_HPP_
#define MESH_HPP_

#include <glad/glad.h>
#include <vector>

namespace Interface {

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
    ~Mesh();

    unsigned int GetVAO() const {
        return vao_;
    }

    unsigned int GetVBO() const {
        return vbo_;
    }

    GLsizei GetVertexCount() const {
        return vertexCount_;
    }

private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    GLsizei vertexCount_ = 0;
};

} // namespace Interface

#endif // MESH_HPP_
