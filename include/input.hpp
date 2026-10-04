#ifndef INPUT_H_
#define INPUT_H_

#include "triangle.hpp"
#include "gl_process.hpp" // TODO: переадресую туда же вопрос с тем, что не нравится

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <optional>

std::optional<std::string> ParseCommandLine(int argc, char** argv);
std::istream* SelectInput(const std::string& path, std::ifstream& file);
std::optional<std::vector<geometry::Triangle>> ReadTriangles(std::istream& in);
void ProcessInput(GLFWwindow* window, Camera& camera);

#endif // INPUT_H_
