#ifndef GEOMETRY_TRIANGLE_SET_HPP_
#define GEOMETRY_TRIANGLE_SET_HPP_

#include <vector>

#include "triangle.hpp"

namespace Geometry {

std::vector<bool> FindIntersectingFlags(const std::vector<Triangle>& triangles);

} // namespace Geometry

#endif // GEOMETRY_TRIANGLE_SET_HPP_
