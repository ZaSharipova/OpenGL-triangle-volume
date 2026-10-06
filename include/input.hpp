#ifndef INPUT_H_
#define INPUT_H_

#include "triangle.hpp"
#include "gl_process.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <fstream>

namespace input {

std::optional<std::vector<geometry::Triangle>> ReadTriangles(std::istream& in);
std::optional<std::vector<geometry::Triangle>> LoadTriangles(int argc, char** argv);

} // namespace input

#endif // INPUT_H_
