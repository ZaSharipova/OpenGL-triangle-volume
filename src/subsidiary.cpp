#include "subsidiary.hpp"

#include <iostream>
#include <vector>
#include <optional>

namespace Subsidiary {

namespace {
    void PushVertex(const Geometry::Vec3& vec, std::vector<float>& points) {
        points.push_back(vec.GetX());
        points.push_back(vec.GetY());
        points.push_back(vec.GetZ());
    }

} // namespace

std::vector<float> FlattenVertices(const std::vector<Geometry::Triangle>& triangles, const std::vector<bool>& intersecting) {
    std::vector<float> flat;
    flat.reserve(triangles.size() * 7 * 3);

    for (size_t tr_index = 0; tr_index < triangles.size(); tr_index++) {
        const Geometry::Triangle& tr = triangles[tr_index];
        Geometry::Vec3 normal = tr.FindNormal().Normalize();

        for (size_t i = 0; i < 3; i++) {
            const Geometry::Vec3& vertex = tr.GetVertex(i);
            flat.push_back(vertex.GetX());
            flat.push_back(vertex.GetY());
            flat.push_back(vertex.GetZ());
            flat.push_back(normal.GetX());
            flat.push_back(normal.GetY());
            flat.push_back(normal.GetZ());
            flat.push_back(intersecting[tr_index]);
        }

    }

    return flat;
}

Matrix4x4 Identity() {
    return {1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
}

Matrix4x4 Perspective(float fov, float aspect, float near, float far) {
    return {1 / (aspect * std::tan(fov / 2)), 0, 0, 0,
            0, 1 / std::tan(fov / 2), 0, 0,
            0, 0, -((far + near) / (far - near)), -(2 * far *near) / (far - near),
            {0, 0, -1, 0}};
}

Matrix4x4 ViewMatrix(const Geometry::Vec3& eye, const Geometry::Vec3& target, const Geometry::Vec3& up) {
    Geometry::Vec3 direction_unnormalized = eye - target;
    Geometry::Vec3 direction = direction_unnormalized.Normalize();

    Geometry::Vec3 right = up.FindCross(direction).Normalize(); // господи как это ужасно ;;;(((
    Geometry::Vec3 up_real = direction.FindCross(right);

    return {right.GetX(), right.GetY(), right.GetZ(), -right.FindDot(eye),
            up_real.GetX(), up_real.GetY(), up_real.GetZ(), -up_real.FindDot(eye),
            direction.GetX(), direction.GetY(), direction.GetZ(), -direction.FindDot(eye),
            0, 0, 0, 1};
}

std::array<Geometry::Vec3, kNumberOfVerticesInCube> FindCubeCorners(const std::pair<Geometry::Vec3, Geometry::Vec3>& boundingBox) {
    std::array<Geometry::Vec3, kNumberOfVerticesInCube> corners;

    for (size_t i = 0; i < kNumberOfVerticesInCube; i++) {
        float x = (i & 1) ? boundingBox.second.GetX() : boundingBox.first.GetX();
        float y = (i & 2) ? boundingBox.second.GetY() : boundingBox.first.GetY();
        float z = (i & 4) ? boundingBox.second.GetZ() : boundingBox.first.GetZ();
        corners[i] = Geometry::Vec3(x, y, z);
    }

    return corners;
}

std::vector<float> FindCubeEdgePoints(const std::array<Geometry::Vec3, kNumberOfVerticesInCube>& corners) {
    std::vector<float> points;
    points.reserve(kNumberOfEdgesInCube * 2 * 3);

    for (size_t i = 0; i < kNumberOfVerticesInCube; i++) {
        for (size_t bit = 0; bit < 3; bit++) {
            size_t j = i ^ (1 << bit);
            if (j > i) {
                PushVertex(corners[i], points);
                PushVertex(corners[j], points);
            }
        }
    }

    return points;
}

} // namespace Subsidiary
