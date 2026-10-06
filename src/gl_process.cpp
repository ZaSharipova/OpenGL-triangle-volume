#include "gl_process.hpp"

#include <assert.h>
#include <numbers>

#include "subsidiary.hpp"
#include "triangle.hpp"
#include "input.hpp"
#include "shaders.hpp"

namespace process {

namespace {

    struct VertexAttribute {
        GLuint location;
        GLint componentCount;
        GLsizei strideFloats;
        size_t offsetFloats;
    };

    struct Mesh {
        unsigned int vao = 0;
        unsigned int vbo = 0;
        GLsizei vertexCount = 0;
    };

    struct Uniforms {
        int model = -1;
        int view = -1;
        int projection = -1;
    };

    void MouseCallback(GLFWwindow* window, double x_coord, double y_coord) {
        Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        if (!cam) {
            std::cerr << "Failed getting a cam\n";
            return;
        }

        if (!cam->cursorIsActive) {
            return;
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

        geometry::Vec3 vec;
        vec.SetX(std::cos(yawR) * std::cos(pitchR));
        vec.SetY(std::sin(pitchR));
        vec.SetZ(std::sin(yawR) * std::cos(pitchR));
        cam->front = vec.Normalize();
    }

    Uniforms FindUniforms(unsigned int program) {
        Uniforms uniforms;
        uniforms.model = glGetUniformLocation(program, "model");
        uniforms.view = glGetUniformLocation(program, "view");
        uniforms.projection = glGetUniformLocation(program, "projection");
        return uniforms;
    }

    void SetMat4Uniform(int location, const subsidiary::Matrix4x4& matrix) {
        glUniformMatrix4fv(location, 1, GL_TRUE, &(matrix.matrix[0][0]));
    }

    void DrawMesh(unsigned int program, const Uniforms& uniforms, const Mesh& mesh,
                  GLenum mode, const subsidiary::Matrix4x4& model, const subsidiary::Matrix4x4& view, const subsidiary::Matrix4x4& projection) {
        glUseProgram(program);
        SetMat4Uniform(uniforms.model, model);
        SetMat4Uniform(uniforms.view, view);
        SetMat4Uniform(uniforms.projection, projection);

        glBindVertexArray(mesh.vao);
        glDrawArrays(mode, 0, mesh.vertexCount);
    }

    unsigned int CompileShader(GLenum type, const char* src) {
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

    unsigned int LinkProgram(unsigned int vertex_shader, unsigned int fragment_shader) {
        unsigned int program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        int linked_success_flag = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linked_success_flag);
        if (!linked_success_flag) {
            char log[LOG_ARRAY_SIZE];
            glGetProgramInfoLog(program, LOG_ARRAY_SIZE, nullptr, log);
            std::cerr << "Link error: " << log << "\n";
        }

        return program;
    }

    unsigned int CreateShaderProgram(const char* vertex_src, const char* fragment_src) {
        unsigned int vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_src);
        unsigned int fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_src);

        unsigned int program = LinkProgram(vertex_shader, fragment_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
    }

    Mesh CreateMesh(const std::vector<float>& data, const std::vector<VertexAttribute>& attributes, GLsizei verticesPerElement) {
        Mesh mesh;
        mesh.vertexCount = static_cast<GLsizei>(data.size()) / verticesPerElement;

        glGenVertexArrays(1, &mesh.vao);
        glGenBuffers(1, &mesh.vbo);

        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), GL_STATIC_DRAW);

        for (const VertexAttribute& attrib : attributes) {
            glVertexAttribPointer(
                attrib.location,
                attrib.componentCount,
                GL_FLOAT,
                GL_FALSE,
                attrib.strideFloats * static_cast<GLsizei>(sizeof(float)),
                reinterpret_cast<void*>(attrib.offsetFloats * sizeof(float)));

            glEnableVertexAttribArray(attrib.location);
        }

        return mesh;
    }

    void DestroyMesh(const Mesh& mesh) {
        glDeleteVertexArrays(1, &mesh.vao);
        glDeleteBuffers(1, &mesh.vbo);
    }

    void ProcessFullScreenToggle(GLFWwindow* window, Camera& camera, const int nowFState) {
        static int prevFState = GLFW_RELEASE;

        if (nowFState == GLFW_PRESS && prevFState != GLFW_PRESS) {
            if (!camera.isFullScreen) {
                glfwGetWindowPos(window, &camera.savedX, &camera.savedY);
                glfwGetWindowSize(window, &camera.savedWidth, &camera.savedHeight);
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(window, monitor, 0, 0,
                    mode->width, mode->height, mode->refreshRate);
                camera.isFullScreen = true;
            } else {
                glfwSetWindowMonitor(window, nullptr, camera.savedX,
                    camera.savedY, camera.savedWidth, camera.savedHeight, 0);
                camera.isFullScreen = false;
            }
        }

        prevFState = nowFState;
    }

    void ProcessMakeCursorFree(GLFWwindow* window, Camera& camera, const int nowTabState) {
        static int prevTabState = GLFW_RELEASE;

        if (nowTabState == GLFW_PRESS && prevTabState != GLFW_PRESS) {
            if (camera.cursorIsActive) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                camera.cursorIsActive = false;
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                camera.cursorIsActive = true;
            }
        }

        prevTabState = nowTabState;
    }

} // namespace

void RunRenderLoop(GLFWwindow* window, const std::vector<float>& triangleVertices,
        const std::vector<float>& cubeEdgePoints) {
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

    const subsidiary::Matrix4x4 model = subsidiary::Identity();
    float aspect = static_cast<float>(kWindowWidth) / kWindowHeight;
    subsidiary::Matrix4x4 projection = subsidiary::Perspective(
        0.785f, aspect, 0.1f, 100.0f);

    Camera camera{};
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, MouseCallback);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ProcessInput(window, camera);

        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        float newAspect = static_cast<float>(width) / height;
        if (newAspect != aspect) {
            projection = subsidiary::Perspective(0.785f, newAspect, 0.1f, 100.0f);
            //std::cerr << width << " x " << height << "\n";
            aspect = newAspect;
        }

        const subsidiary::Matrix4x4 view = subsidiary::ViewMatrix(camera.pos, camera.pos + camera.front, camera.up);

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
}

void ProcessInput(GLFWwindow* window, Camera& camera) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    ProcessMakeCursorFree(window, camera, glfwGetKey(window, GLFW_KEY_TAB));

    ProcessFullScreenToggle(window, camera, glfwGetKey(window, GLFW_KEY_F));

    const float cameraSpeed = 0.05f;
    geometry::Vec3 right = camera.front.FindCross(camera.up).Normalize();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.pos = camera.pos + camera.front * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.pos = camera.pos - camera.front * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.pos = camera.pos - right * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.pos = camera.pos + right * cameraSpeed;
    }
}

GLFWwindow* CreateGLWindow(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Failed to init glfw\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "failed to initialize GLAD\n";
        return nullptr;
    }

    return window;
}

} // namespace process
