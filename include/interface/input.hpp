#ifndef INPUT_H_
#define INPUT_H_

#include "config.hpp"
#include "triangle.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

namespace Interface {

namespace Input {

std::optional<std::vector<Geometry::Trianglef>> ReadTriangles(std::istream& in);
std::optional<std::vector<Geometry::Trianglef>> LoadTriangles(int argc, char** argv);

} // namespace Input

} // namespace Interface

#endif // INPUT_H_
