#ifndef GEOMETRY_SUBSIDIARY_HPP_
#define GEOMETRY_SUBSIDIARY_HPP_

#include "vec3.hpp"
#include "triangle.hpp"

namespace Geometry {

inline constexpr float kEps = 1e-6f;

bool AreEqual(const float number_1, const float number_2);
bool IsZero(float vec);
bool IsZero(const Vec3& vec);
std::pair<Vec3, Vec3> FindBoundingBox(const Triangle& triangle);
std::pair<Vec3, Vec3> FindBoundingBox(const std::vector<Triangle>& triangles);

} // namespace Geometry

#endif // GEOMETRY_SUBSIDIARY_HPP_
