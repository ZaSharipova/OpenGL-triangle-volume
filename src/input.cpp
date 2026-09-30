#include "input.hpp"

#include "triangle.hpp"

#include <iostream>
#include <optional>

std::optional<std::vector<geometry::Triangle>> ReadTriangles(std::istream& in) {
    size_t size = 0;
    if (!(in >> size)) {
        std::cerr << "Failed to read size\n";
        return std::nullopt;
    }

    std::vector<geometry::Triangle> triangles;
    triangles.reserve(size);

    for (size_t i = 0; i < size; i++) {
        float line[9] {};
        for (float& value : line) {
            if (!(in >> value)) {
                std::cerr << "Failed to read triangle #" + std::to_string(i);
                return std::nullopt;
            }
        }

        triangles.emplace_back(geometry::Vec3(line[0], line[1], line[2]),
                               geometry::Vec3(line[3], line[4], line[5]),
                               geometry::Vec3(line[6], line[7], line[8]));
    }

    return triangles;
}

void ProcessInput(GLFWwindow* window, Camera& camera) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    const float cameraSpeed = 0.05f;
    geometry::Vec3 right = camera.front.FindCross(camera.up).Normalize();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.pos = camera.pos + camera.front * cameraSpeed; // TODO
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.pos = camera.pos - camera.front * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.pos = camera.pos + right * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.pos = camera.pos - right * cameraSpeed;
    }
}
