#ifndef SUBSIDIARY_HPP_
#define SUBSIDIARY_HPP_

#include "triangle.hpp"
#include "vec3.hpp"

#include <vector>

namespace Subsidiary {

constexpr size_t kNumberOfVerticesInCube = 8;
constexpr size_t kNumberOfEdgesInCube = 12;

std::vector<float> FlattenVertices(const std::vector<Geometry::Triangle>& triangles, const std::vector<bool>& intersecting);

struct Matrix4x4 {
    float matrix[4][4];
};

Matrix4x4 Identity();
Matrix4x4 Perspective(float fov, float aspect, float near, float far);
Matrix4x4 ViewMatrix(const Geometry::Vec3& eye, const Geometry::Vec3& target, const Geometry::Vec3& up);
std::array<Geometry::Vec3, kNumberOfVerticesInCube> FindCubeCorners(const std::pair<Geometry::Vec3, Geometry::Vec3>& boundingBox);
std::vector<float> FindCubeEdgePoints(const std::array<Geometry::Vec3, kNumberOfVerticesInCube>& corners);

} // namespace Subsidiary

#endif // SUBSIDIARY_HPP_
