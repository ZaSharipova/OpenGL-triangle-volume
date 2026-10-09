#ifndef INPUT_H_
#define INPUT_H_

#include "triangle.hpp"

#include <optional>
#include <fstream>

namespace Input {

std::optional<std::vector<Geometry::Triangle>> ReadTriangles(std::istream& in);
std::optional<std::vector<Geometry::Triangle>> LoadTriangles(int argc, char** argv);

} // namespace Input

#endif // INPUT_H_
