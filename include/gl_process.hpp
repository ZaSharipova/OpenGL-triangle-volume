#ifndef GL_PROCESS_H_
#define GL_PROCESS_H_

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "subsidiary.hpp"

namespace process {

constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;
constexpr size_t LOG_ARRAY_SIZE = 512;

struct Camera {
    geometry::Vec3 pos {0.0f, 0.0f, 3.0f};
    geometry::Vec3 front {0.0f, 0.0f, -1.0f};
    geometry::Vec3 up {0.0f, 1.0f, 0.0f};

    float yaw = -90.0f;
    float pitch = 0.0f;

    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;

    float mouseSensitivity = 0.1f;
};

void RunRenderLoop(GLFWwindow* window, const std::vector<float>& triangleVertices,
        const std::vector<float>& cubeEdgePoints);
void ProcessInput(GLFWwindow* window, Camera& camera);
GLFWwindow* CreateGLWindow(int width, int height, const char* title);

} // namespace process

#endif // GL_PROCESS_H_
