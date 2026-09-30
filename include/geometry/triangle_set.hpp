#ifndef GEOMETRY_TRIANGLE_SET_HPP_
#define GEOMETRY_TRIANGLE_SET_HPP_

#include <vector>

#include "triangle.hpp"

namespace geometry {

std::vector<bool> FindIntersectingFlags(const std::vector<Triangle>& triangles);

} // namespace geometry

#endif // GEOMETRY_TRIANGLE_SET_HPP_
