#ifndef SUBSIDIARY_HPP_
#define SUBSIDIARY_HPP_

#include "triangle.hpp"
#include "vec3.hpp"

#include <vector>

namespace subsidiary {

constexpr size_t kNumberOfVerticesInCube = 8;
constexpr size_t kNumberOfEdgesInCube = 12;

std::vector<float> FlattenVertices(const std::vector<geometry::Triangle>& triangles, const std::vector<bool>& intersecting);

struct Matrix4x4 {
    float matrix[4][4];
};

Matrix4x4 Identity();
Matrix4x4 Perspective(float fov, float aspect, float near, float far);
Matrix4x4 ViewMatrix(const geometry::Vec3& eye, const geometry::Vec3& target, const geometry::Vec3& up);
std::array<geometry::Vec3, kNumberOfVerticesInCube> FindCubeCorners(const std::pair<geometry::Vec3, geometry::Vec3>& boundingBox);
std::vector<float> FindCubeEdgePoints(const std::array<geometry::Vec3, kNumberOfVerticesInCube>& corners);

} // namespace subsidiary

#endif // SUBSIDIARY_HPP_
