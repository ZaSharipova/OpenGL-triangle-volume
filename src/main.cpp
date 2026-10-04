
#include <cassert>
#include <vector>
#include <optional>
#include <fstream>

#include "geometry_subsidiary.hpp"
#include "subsidiary.hpp"
#include "triangle.hpp"
#include "input.hpp"

int main(int argc, char** argv) {
    auto triangles = input::LoadTriangles(argc, argv);
    if (!triangles.has_value()) {
        return 1;
    }

    const std::vector<bool> intersecting = geometry::FindIntersectingFlags(*triangles);
    const auto boundingBox = geometry::FindBoundingBox(*triangles);
    const auto cubeCorners = subsidiary::FindCubeCorners(boundingBox);
    const std::vector<float> cubeEdgePoints = subsidiary::FindCubeEdgePoints(cubeCorners);
    const std::vector<float> triangleVertices = subsidiary::FlattenVertices(*triangles, intersecting);

    GLFWwindow* window = process::CreateGLWindow(process::kWindowWidth, process::kWindowHeight, "OpenGL Window");
    if (!window) {
        return 1;
    }

    process::RunRenderLoop(window, triangleVertices, cubeEdgePoints);

    glfwTerminate();
    return 0;
}
