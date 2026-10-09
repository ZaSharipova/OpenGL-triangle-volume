#include "window.hpp"

#include <iostream>
#include <stdexcept>

Window::Window(int width, int height, const char *title) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to init glfw\n");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("failed to initialize GLAD\n");
    }
}

GLFWwindow* Window::GetWindow() {
    return window_;
}

int& Window::GetSavedWidth() {
    return savedWidth_;
}

int& Window::GetSavedHeight() {
    return savedHeight_;
}

int& Window::GetSavedX() {
    return savedX_;
}

int& Window::GetSavedY() {
    return savedY_;
}

bool& Window::GetIsFullScreen() {
    return isFullScreen_;
}

void Window::SetCursorCaptured(bool captured) {
    glfwSetInputMode(window_, GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    cursorCaptured_ = captured;
}

bool Window::IsCursorCaptured() const {
    return cursorCaptured_;
}

void Window::ToggleFullScreen() {
    if (!isFullScreen_) {
        glfwGetWindowPos(window_, &savedX_, &savedY_);
        glfwGetWindowSize(window_, &savedWidth_, &savedHeight_);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window_, monitor, 0, 0,
            mode->width, mode->height, mode->refreshRate);
        isFullScreen_ = true;
    } else {
        glfwSetWindowMonitor(window_, nullptr, savedX_,
            savedY_, savedWidth_, savedHeight_, 0);
        isFullScreen_ = false;
    }
}
