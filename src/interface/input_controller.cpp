#include "input_controller.hpp"

#include "camera.hpp"
#include "window.hpp"

#include <iostream>
#include <numbers>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void InputController::ProcessInput(Window& window, Camera& camera) {
    if (glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window.GetWindow(), true);
    }

    ProcessMakeCursorFree(window, glfwGetKey(window.GetWindow(), GLFW_KEY_TAB));
    ProcessFullScreenToggle(window, glfwGetKey(window.GetWindow(), GLFW_KEY_F));

    const float cameraSpeed = 0.05f;
    Geometry::Vec3 right = camera.front.FindCross(camera.up).Normalize();

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        camera.pos = camera.pos + camera.front * cameraSpeed;
    }
    if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        camera.pos = camera.pos - camera.front * cameraSpeed;
    }
    if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        camera.pos = camera.pos - right * cameraSpeed;
    }
    if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        camera.pos = camera.pos + right * cameraSpeed;
    }
}

void InputController::ProcessMouse(Camera& cam, double xpos, double ypos) {
    const auto x = static_cast<float>(xpos);
    const auto y = static_cast<float>(ypos);

    if (firstMouse_) {
        lastX_ = x;
        lastY_ = y;
        firstMouse_ = false;
        return;
    }

    float dx = x - lastX_;
    float dy = -y + lastY_;
    lastX_ = x;
    lastY_ = y;

    cam.yaw += dx * mouseSensitivity_;
    cam.pitch += dy * mouseSensitivity_;

    if (cam.pitch > 89.0f) {
        cam.pitch = 89.0f;
    }

    if (cam.pitch < -89.0f) {
        cam.pitch = -89.0f;
    }

    const float toRad = std::numbers::pi / 180.0f;
    float yawR = cam.yaw * toRad;
    float pitchR = cam.pitch * toRad;

    Geometry::Vec3 vec;
    vec.SetX(std::cos(yawR) * std::cos(pitchR));
    vec.SetY(std::sin(pitchR));
    vec.SetZ(std::sin(yawR) * std::cos(pitchR));
    cam.front = vec.Normalize();
}

void InputController::ProcessFullScreenToggle(Window& window, const int nowFState) {
    if (nowFState == GLFW_PRESS && prevFState_ != GLFW_PRESS) {
        window.ToggleFullScreen();
    }

    prevFState_ = nowFState;
}

void InputController::ProcessMakeCursorFree(Window& window, const int nowTabState) {
    if (nowTabState == GLFW_PRESS && prevTabState_ != GLFW_PRESS) {
        window.SetCursorCaptured(!window.IsCursorCaptured());
        if (window.IsCursorCaptured()) {
            ResetMouse();
        }
    }

    prevTabState_ = nowTabState;
}

void InputController::ResetMouse() {
    firstMouse_ = true;
}
