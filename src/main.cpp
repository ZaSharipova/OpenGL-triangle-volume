
#include <cassert>
#include <vector>
#include <optional>
#include <fstream>

#include "geometry_subsidiary.hpp"
#include "subsidiary.hpp"
#include "triangle.hpp"
#include "input.hpp"
#include "shaders.hpp"

int main(int argc, char** argv) {
    std::optional<std::string> parse_result = ParseCommandLine(argc, argv);
    if (!parse_result.has_value()) {
        return 1;
    }

    std::ifstream file;
    std::istream* stream = SelectInput(parse_result.value(), file);
    if (!stream) {
        std::cerr << "error: cannot open file\n";
        return 1;
    }

    std::optional<std::vector<geometry::Triangle>> result = ReadTriangles(*stream);
    if (result == std::nullopt) {
        return -1;
    }
    std::vector<geometry::Triangle> triangles = result.value();

    std::vector<bool> intersecting = FindIntersectingFlags(triangles);

    std::pair<geometry::Vec3, geometry::Vec3> boundingBox = FindBoundingBox(triangles);
    std::array<geometry::Vec3, kNumberOfVerticesInCube> cubeCorners = FindCubeCorners(boundingBox);
    std::vector<float> cubeEdgePoints = FindCubeEdgePoints(cubeCorners);
    std::vector<float> triangleVertices = FlattenVertices(triangles, intersecting);

    GLFWwindow* window = CreateGLWindow(800, 600, "OpenGL Window");
    if (!window) {
        return -1;
    }

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

    Matrix4x4 model = Identity();
    Matrix4x4 projection = Perspective(0.785f, 800.0f / 600.0f, 0.1f, 100.0f);

    Camera camera{};
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ProcessInput(window, camera);

        Matrix4x4 view = ViewMatrix(camera.pos, camera.pos + camera.front, camera.up);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrawMesh(triangleProgram, triangleUniforms, triangleMesh, GL_TRIANGLES, model, view, projection);
        DrawMesh(cubeProgram, cubeUniforms, cubeMesh, GL_LINES, model, view, projection);

        glfwSwapBuffers(window);
    }

    DestroyMesh(triangleMesh);
    DestroyMesh(cubeMesh);
    glDeleteProgram(triangleProgram);
    glDeleteProgram(cubeProgram);
    glfwTerminate();
    return 0;
}
