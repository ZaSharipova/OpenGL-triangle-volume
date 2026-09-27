#ifndef INPUT_H_
#define INPUT_H_

#include "triangle.hpp"
#include "gl_process.hpp" // TODO: переадресую туда же вопрос с тем, что не нравится

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::optional<std::vector<Triangle>> ReadTriangles(std::istream& in);
void ProcessInput(GLFWwindow* window, Camera& camera);

#endif // INPUT_H_
