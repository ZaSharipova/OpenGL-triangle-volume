#ifndef GLFW_CONTEXT_HPP_
#define GLFW_CONTEXT_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

struct GLFWContext {
    GLFWContext() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to init glfw\n");
        }
    }

    ~GLFWContext() {
        glfwTerminate();
    }

    GLFWContext(const GLFWContext&) = delete;
    GLFWContext& operator=(const GLFWContext&) = delete;
};

#endif // GLFW_CONTEXT_HPP_
