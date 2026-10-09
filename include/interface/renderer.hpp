#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "subsidiary.hpp"
#include "mesh.hpp"
#include "window.hpp"

namespace Renderer {

struct Uniforms {
    int model = -1;
    int view = -1;
    int projection = -1;
};

Uniforms FindUniforms(unsigned int program);
void SetMat4Uniform(int location, const Subsidiary::Matrix4x4& matrix);
void DrawMesh(unsigned int program, const Uniforms& uniforms, const Mesh& mesh, GLenum mode, const Subsidiary::Matrix4x4& model,
              const Subsidiary::Matrix4x4& view, const Subsidiary::Matrix4x4& projection);

void RunRenderLoop(Window& window, const std::vector<float>& triangleVertices,
                   const std::vector<float>& cubeEdgePoints);

} // namespace Renderer

#endif // RENDERER_HPP_
