
#include <cassert>
#include <fstream>
#include <optional>
#include <vector>

#include "geometry_subsidiary.hpp"
#include "glfw_context.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "subsidiary.hpp"
#include "triangle.hpp"
#include "triangle_set.hpp"
#include "window.hpp"

int main(int argc, char** argv) {
    auto triangles = Interface::Input::LoadTriangles<float>(argc, argv);
    if (!triangles.has_value()) {
        return 1;
    }

    const std::vector<bool> intersecting = Geometry::FindIntersectingFlags(*triangles);
    const auto boundingBox = Geometry::FindBoundingBox(*triangles);
    const auto cubeCorners = Interface::Subsidiary::FindCubeCorners(boundingBox);
    const std::vector<float> cubeEdgePoints = Interface::Subsidiary::FindCubeEdgePoints(cubeCorners);
    const std::vector<float> triangleVertices = Interface::Subsidiary::FlattenVertices(*triangles, intersecting);

    GLFWContext context;
    Interface::Window window(Interface::Window::kWindowWidth, Interface::Window::kWindowHeight, "OpenGL Window");
    Interface::Renderer::RunRenderLoop(window, triangleVertices, cubeEdgePoints);

    return 0;
}
