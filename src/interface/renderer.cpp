#include "renderer.hpp"

#include <vector>

#include "camera.hpp"
#include "window.hpp"
#include "shader_program.hpp"
#include "shaders.hpp"
#include "input_controller.hpp"

namespace Renderer {

namespace {

struct MouseContext {
    InputController* controller;
    Camera* camera;
    Window* window;
};

void MouseBridge(GLFWwindow* window, double xpos, double ypos) {
    auto* ctx = static_cast<MouseContext*>(glfwGetWindowUserPointer(window));
    if (!ctx || !ctx->controller || !ctx->camera || !ctx->window) {
        return;
    }

    if (!ctx->window->IsCursorCaptured()) {
        return;
    }

    ctx->controller->ProcessMouse(*ctx->camera, xpos, ypos);
}

} // namespace

void RunRenderLoop(Window& window, const std::vector<float>& triangleVertices,
        const std::vector<float>& cubeEdgePoints) {

    unsigned int triangleProgram = CreateShaderProgram(trianglefVertexShaderSrc, trianglefFragmentShaderSrc);
    unsigned int cubeProgram = CreateShaderProgram(cubeVertexShaderSrc, cubeFragmentShaderSrc);

    Uniforms triangleUniforms = FindUniforms(triangleProgram);
    Uniforms cubeUniforms = FindUniforms(cubeProgram);

    Mesh triangleMesh = CreateMesh(triangleVertices, {
        {0, 3, 7, 0},
        {1, 3, 7, 3},
        {2, 1, 7, 6},
    }, 7);

    Mesh cubeMesh = CreateMesh(cubeEdgePoints, {
        {0, 3, 3, 0},
    }, 3);

    glEnable(GL_DEPTH_TEST);

    const Subsidiary::Matrix4x4 model = Subsidiary::Identity();
    float aspect = static_cast<float>(Window::kWindowWidth) / Window::kWindowHeight;
    Subsidiary::Matrix4x4 projection = Subsidiary::Perspective(
        0.785f, aspect, 0.1f, 100.0f);

    Camera camera{};
    InputController input_controller{};

    MouseContext mouseContext{&input_controller, &camera, &window};
    glfwSetWindowUserPointer(window.GetWindow(), &mouseContext);
    glfwSetCursorPosCallback(window.GetWindow(), MouseBridge);

    while (!glfwWindowShouldClose(window.GetWindow())) {
        glfwPollEvents();
        input_controller.ProcessInput(window, camera);

        int width = 0, height = 0;
        glfwGetFramebufferSize(window.GetWindow(), &width, &height);
        glViewport(0, 0, width, height);

        float newAspect = static_cast<float>(width) / height;
        if (newAspect != aspect) {
            projection = Subsidiary::Perspective(0.785f, newAspect, 0.1f, 100.0f);
            aspect = newAspect;
        }

        const Subsidiary::Matrix4x4 view = Subsidiary::ViewMatrix(camera.pos, camera.pos + camera.front, camera.up);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrawMesh(triangleProgram, triangleUniforms, triangleMesh, GL_TRIANGLES, model, view, projection);
        DrawMesh(cubeProgram, cubeUniforms, cubeMesh, GL_LINES, model, view, projection);

        glfwSwapBuffers(window.GetWindow());
    }

    DestroyMesh(triangleMesh);
    DestroyMesh(cubeMesh);
    glDeleteProgram(triangleProgram);
    glDeleteProgram(cubeProgram);
}

Uniforms FindUniforms(unsigned int program) {
    Uniforms uniforms;
    uniforms.model = glGetUniformLocation(program, "model");
    uniforms.view = glGetUniformLocation(program, "view");
    uniforms.projection = glGetUniformLocation(program, "projection");
    return uniforms;
}

void SetMat4Uniform(int location, const Subsidiary::Matrix4x4& matrix) {
    glUniformMatrix4fv(location, 1, GL_TRUE, &(matrix.matrix[0][0]));
}

void DrawMesh(unsigned int program, const Uniforms& uniforms, const Mesh& mesh, GLenum mode, const Subsidiary::Matrix4x4& model,
              const Subsidiary::Matrix4x4& view, const Subsidiary::Matrix4x4& projection) {
    glUseProgram(program);
    SetMat4Uniform(uniforms.model, model);
    SetMat4Uniform(uniforms.view, view);
    SetMat4Uniform(uniforms.projection, projection);

    glBindVertexArray(mesh.vao);
    glDrawArrays(mode, 0, mesh.vertexCount);
}

} // namespace Renderer
