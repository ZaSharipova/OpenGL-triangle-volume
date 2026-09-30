#include "subsidiary.hpp"

#include <iostream>
#include <vector>
#include <optional>

namespace {
    void PushVertex(const geometry::Vec3& vec, std::vector<float>& points) {
        points.push_back(vec.GetX());
        points.push_back(vec.GetY());
        points.push_back(vec.GetZ());
    }

} // namespace

std::vector<float> FlattenVertices(const std::vector<geometry::Triangle>& triangles, const std::vector<bool>& intersecting) {
    std::vector<float> flat;
    flat.reserve(triangles.size() * 7 * 3);

    for (size_t tr_index = 0; tr_index < triangles.size(); tr_index++) {
        const geometry::Triangle& tr = triangles[tr_index];
        geometry::Vec3 normal = tr.FindNormal().Normalize();

        for (size_t i = 0; i < 3; i++) {
            const geometry::Vec3& vertex = tr.GetVertex(i);
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

Matrix4x4 ViewMatrix(const geometry::Vec3& eye, const geometry::Vec3& target, const geometry::Vec3& up) {
    geometry::Vec3 direction_unnormalized = eye - target;
    geometry::Vec3 direction = direction_unnormalized.Normalize();

    geometry::Vec3 right = up.FindCross(direction).Normalize(); // господи как это ужасно ;;;(((
    geometry::Vec3 up_real = direction.FindCross(right);

    return {right.GetX(), right.GetY(), right.GetZ(), -right.FindDot(eye),
            up_real.GetX(), up_real.GetY(), up_real.GetZ(), -up_real.FindDot(eye),
            direction.GetX(), direction.GetY(), direction.GetZ(), -direction.FindDot(eye),
            0, 0, 0, 1};
}

std::pair<geometry::Vec3, geometry::Vec3> FindBoundingBox(const std::vector<geometry::Triangle>& triangles) {
    constexpr float kInf = std::numeric_limits<float>::infinity();
    std::array<float, 3> min_coord = {kInf, kInf, kInf};
    std::array<float, 3> max_coord = {-kInf, -kInf, -kInf};

    for (const geometry::Triangle& triangle : triangles) {
        for (size_t i = 0; i < 3; i++) {
            const geometry::Vec3& v = triangle.GetVertex(i);
            const std::array<float, 3> coords = {v.GetX(), v.GetY(), v.GetZ()};
            for (size_t axis = 0; axis < 3; axis++) {
                min_coord[axis] = std::min(min_coord[axis], coords[axis]);
                max_coord[axis] = std::max(max_coord[axis], coords[axis]);
            }
        }
    }

    return { geometry::Vec3(min_coord[0], min_coord[1], min_coord[2]),
             geometry::Vec3(max_coord[0], max_coord[1], max_coord[2])};
}

std::array<geometry::Vec3, kNumberOfVerticesInCube> FindCubeCorners(const std::pair<geometry::Vec3, geometry::Vec3>& boundingBox) {
    std::array<geometry::Vec3, kNumberOfVerticesInCube> corners;

    for (size_t i = 0; i < kNumberOfVerticesInCube; i++) {
        float x = (i & 1) ? boundingBox.second.GetX() : boundingBox.first.GetX();
        float y = (i & 2) ? boundingBox.second.GetY() : boundingBox.first.GetY();
        float z = (i & 4) ? boundingBox.second.GetZ() : boundingBox.first.GetZ();
        corners[i] = geometry::Vec3(x, y, z);
    }

    return corners;
}

std::vector<float> FindCubeEdgePoints(const std::array<geometry::Vec3, kNumberOfVerticesInCube>& corners) {
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
