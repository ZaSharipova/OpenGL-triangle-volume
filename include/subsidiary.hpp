#ifndef SUBSIDIARY_HPP_
#define SUBSIDIARY_HPP_

#include "triangle.hpp"

#include <optional>
#include <vector>

constexpr size_t kNumberOfVerticesInCube = 8;
constexpr size_t kNumberOfEdgesInCube = 12;

std::vector<float> FlattenVertices(const std::vector<Triangle>& triangles, const std::vector<bool>& intersecting);

struct Matrix4x4 {
    float matrix[4][4];
};

Matrix4x4 Identity();
Matrix4x4 Perspective(float fov, float aspect, float near, float far);
Matrix4x4 ViewMatrix(const Vec3& eye, const Vec3& target, const Vec3& up);
std::pair<Vec3, Vec3> FindBoundingBox(const std::vector<Triangle>& triangles);
std::array<Vec3, kNumberOfVerticesInCube> FindCubeCorners(const std::pair<Vec3, Vec3>& boundingBox);
std::vector<float> FindCubeEdgePoints(const std::array<Vec3, kNumberOfVerticesInCube>& corners);

#endif // SUBSIDIARY_HPP_
