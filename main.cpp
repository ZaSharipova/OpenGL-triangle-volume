#include <iostream>
#include <cassert>
#include <numbers>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>

#include "subsidiary.hpp"
#include "triangle.hpp"

const size_t LOG_ARRAY_SIZE = 512;

const char *vertexShaderSrc = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in float aFlag;\n"
    "out float vFlag;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 projection;"
    "void main() {\n"
    "   vFlag = aFlag;\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);"
    "}";

const char *fragmentShaderSrc = "#version 330 core\n"
    "in float vFlag;\n"
    "out vec4 FlagColor;\n"
    "void main() {\n"
    "   vec3 color = vFlag > 0 ? vec3(1.0, 0.2, 0.2) : vec3(0.2, 0.6, 1.0);\n"
    "   FlagColor = vec4(color, 1.0);"
    "}";

unsigned int compileShader(GLenum type, const char* src) {
    assert(src);

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success_flag = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success_flag);
    if (!success_flag) {
        char* log = new char [LOG_ARRAY_SIZE];
        glGetShaderInfoLog(shader, LOG_ARRAY_SIZE, nullptr, log);
        std::cerr << "Shader error: " << log << "\n";
        delete[] log;
    }

    return shader;
}

void MouseCallback(GLFWwindow* window, double x_coord, double y_coord) {
    Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (!cam) {
        std::cerr << "Failed getting a cam\n";
        return; // TODO
    }

    if (cam->firstMouse) {
        cam->lastX = static_cast<float>(x_coord);
        cam->lastY = static_cast<float>(y_coord);
        cam->firstMouse = false;
        return;
    }

    float dx = static_cast<float>(x_coord) - cam->lastX;
    float dy = -static_cast<float>(y_coord) + cam->lastY;
    cam->lastX = static_cast<float>(x_coord);
    cam->lastY = static_cast<float>(y_coord);

    cam->yaw += dx * cam->mouseSensitivity;
    cam->pitch += dy * cam->mouseSensitivity;

    if (cam->pitch > 89.0f) {
        cam->pitch = 89.0f;
    }

    if (cam->pitch < -89.0f) {
        cam->pitch = -89.0f;
    }

    const float toRad = std::numbers::pi / 180.0f;
    float yawR = cam->yaw * toRad;
    float pitchR = cam->pitch * toRad;

    Vec3 vec;
    vec.SetX(std::cos(yawR) * std::cos(pitchR));
    vec.SetY(std::sin(pitchR));
    vec.SetZ(std::sin(yawR) * std::cos(pitchR));
    cam->front = vec.Normalize();
}

void ProcessInput(GLFWwindow* window, Camera& camera) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    const float cameraSpeed = 0.05f;
    Vec3 right = camera.front.FindCross(camera.up).Normalize();

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

int main(void) {
    std::vector<Triangle> triangles;
    std::optional<std::vector<Triangle>> result = ReadTriangles(std::cin);
    if (result == std::nullopt) {
        return -1;
    }
    triangles = result.value();

    size_t size = triangles.size();
    std::vector<bool> intersecting(size, false);
    for (size_t i = 0; i < size; i++) {
        for (size_t j = i + 1; j < size; j++) {
            if (triangles[i].HaveIntersection(triangles[j])) {
                intersecting[i] = intersecting[j] = true;
            }
        }
    }

    if (!glfwInit()) {
        std::cerr << "Failed to init glfw\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "failed to initialize GLAD\n";
        return -1;
    }

    unsigned int vertex_shader = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    unsigned int fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    int linked_success_flag = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked_success_flag);
    if (!linked_success_flag) {
        char* log = new char [LOG_ARRAY_SIZE];
        glGetShaderInfoLog(program, LOG_ARRAY_SIZE, nullptr, log);
        std::cerr << "Link error: " << log << "\n";
        delete[] log;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int loc_model = glGetUniformLocation(program, "model");
    int loc_view = glGetUniformLocation(program, "view");
    int loc_projection = glGetUniformLocation(program, "projection");
    Matrix4x4 model = Identity();
    Matrix4x4 projection = Perspective(0.785f, 800.0f / 600.0f, 0.1f, 100.0f);

    std::vector<float> flat = FlattenVertices(triangles, intersecting);

    unsigned int VAO = 0, VBO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(flat.size() * sizeof(float)), flat.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    Camera camera {};
    glfwSetWindowUserPointer(window, &camera);

    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ProcessInput(window, camera);
        Matrix4x4 view = ViewMatrix(camera.pos, camera.pos + camera.front, camera.up);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, &(model.matrix[0][0]));
        glUniformMatrix4fv(loc_view, 1, GL_TRUE, &(view.matrix[0][0]));
        glUniformMatrix4fv(loc_projection, 1, GL_TRUE, &(projection.matrix[0][0]));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(triangles.size() * 3));

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}
