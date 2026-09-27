#ifndef GL_PROCESS_H_
#define GL_PROCESS_H_

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "subsidiary.hpp"

const size_t LOG_ARRAY_SIZE = 512;

unsigned int CompileShader(GLenum type, const char* src);
unsigned int LinkProgram(unsigned int vertex_shader, unsigned int fragment_shader);
unsigned int CreateShaderProgram(const char* vertex_src, const char* fragment_src);

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

struct Uniforms {
    int model = -1;
    int view = -1;
    int projection = -1;
};

struct Camera { // не нравится так, надо сделать по-другому
    Vec3 pos {0.0f, 0.0f, 3.0f};
    Vec3 front {0.0f, 0.0f, -1.0f};
    Vec3 up {0.0f, 1.0f, 0.0f};

    float yaw = -90.0f;
    float pitch = 0.0f;

    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;

    float mouseSensitivity = 0.1f;
};

Mesh CreateMesh(const std::vector<float>& data, const std::vector<VertexAttribute>& attributes, GLsizei verticesPerElement);
void DestroyMesh(const Mesh& mesh);
GLFWwindow* CreateGLWindow(int width, int height, const char* title);
void MouseCallback(GLFWwindow* window, double x_coord, double y_coord);
Uniforms FindUniforms(unsigned int program);
void SetMat4Uniform(int location, const Matrix4x4& matrix);
void DrawMesh(unsigned int program, const Uniforms& uniforms, const Mesh& mesh,
              GLenum mode, const Matrix4x4& model, const Matrix4x4& view, const Matrix4x4& projection);


#endif // GL_PROCESS_H_
