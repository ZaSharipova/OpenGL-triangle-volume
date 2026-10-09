
#include <cassert>
#include <vector>
#include <optional>
#include <fstream>

#include "geometry_subsidiary.hpp"
#include "subsidiary.hpp"
#include "triangle.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "window.hpp"

int main(int argc, char** argv) {
    auto triangles = Input::LoadTriangles(argc, argv);
    if (!triangles.has_value()) {
        return 1;
    }

    const std::vector<bool> intersecting = Geometry::FindIntersectingFlags(*triangles);
    const auto boundingBox = Geometry::FindBoundingBox(*triangles);
    const auto cubeCorners = Subsidiary::FindCubeCorners(boundingBox);
    const std::vector<float> cubeEdgePoints = Subsidiary::FindCubeEdgePoints(cubeCorners);
    const std::vector<float> triangleVertices = Subsidiary::FlattenVertices(*triangles, intersecting);

    Window window(Window::kWindowWidth, Window::kWindowHeight, "OpenGL Window");
    Renderer::RunRenderLoop(window, triangleVertices, cubeEdgePoints);

    glfwTerminate();
    return 0;
}
