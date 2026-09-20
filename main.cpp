#include <iostream>
#include <cassert>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const size_t LOG_ARRAY_SIZE = 512;

const char *vertexShaderSrc = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos, 1.0);"
    "}";

const char *fragmentShaderSrc = "#version 330 core\n"
    "out vec4 FlagColor;\n"
    "void main() {\n"
    "   FlagColor = vec4(2.0, 0.5, 0.2, 0.1);"
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

int main(void) {
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

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
    };

    unsigned int VAO = 0, VBO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}
