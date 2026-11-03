#include "subsidiary.hpp"
#include "config.hpp"

#include <iostream>
#include <optional>
#include <vector>

namespace Interface {

namespace Subsidiary {

namespace {
void PushVertex(const Geometry::Vec3f& vec, std::vector<float>& points) {
    points.push_back(vec.x_);
    points.push_back(vec.y_);
    points.push_back(vec.z_);
}

} // namespace

std::vector<float> FlattenVertices(const std::vector<Geometry::Trianglef>& triangles, const std::vector<bool>& intersecting) {
    std::vector<float> flat;
    flat.reserve(triangles.size() * 7 * 3);

    for (size_t tr_index = 0; tr_index < triangles.size(); tr_index++) {
        const Geometry::Trianglef& tr = triangles[tr_index];
        Geometry::Vec3f normal = tr.FindNormal().Normalize();

        for (size_t i = 0; i < 3; i++) {
            const Geometry::Vec3f& vertex = tr.GetVertex(i);
            flat.push_back(vertex.x_);
            flat.push_back(vertex.y_);
            flat.push_back(vertex.z_);
            flat.push_back(normal.x_);
            flat.push_back(normal.y_);
            flat.push_back(normal.z_);
            flat.push_back(intersecting[tr_index]);
        }
    }

    return flat;
}

Matrix4x4 Identity() {
    return { 1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 };
}

Matrix4x4 Perspective(float fov, float aspect, float near, float far) {
    return { 1 / (aspect * std::tan(fov / 2)), 0, 0, 0,
        0, 1 / std::tan(fov / 2), 0, 0,
        0, 0, -((far + near) / (far - near)), -(2 * far * near) / (far - near),
        { 0, 0, -1, 0 } };
}

Matrix4x4 ViewMatrix(const Geometry::Vec3f& eye, const Geometry::Vec3f& target, const Geometry::Vec3f& up) {
    Geometry::Vec3f direction_unnormalized = eye - target;
    Geometry::Vec3f direction = direction_unnormalized.Normalize();

    Geometry::Vec3f right = up.FindCross(direction).Normalize();
    Geometry::Vec3f up_real = direction.FindCross(right);

    return { right.x_, right.y_, right.z_, -right.FindDot(eye),
        up_real.x_, up_real.y_, up_real.z_, -up_real.FindDot(eye),
        direction.x_, direction.y_, direction.z_, -direction.FindDot(eye),
        0, 0, 0, 1 };
}

std::array<Geometry::Vec3f, kNumberOfVerticesInCube> FindCubeCorners(const std::pair<Geometry::Vec3f, Geometry::Vec3f>& boundingBox) {
    std::array<Geometry::Vec3f, kNumberOfVerticesInCube> corners;

    for (size_t i = 0; i < kNumberOfVerticesInCube; i++) {
        float x = (i & 1) ? boundingBox.second.x_ : boundingBox.first.x_;
        float y = (i & 2) ? boundingBox.second.y_ : boundingBox.first.y_;
        float z = (i & 4) ? boundingBox.second.z_ : boundingBox.first.z_;
        corners[i] = Geometry::Vec3f(x, y, z);
    }

    return corners;
}

std::vector<float> FindCubeEdgePoints(const std::array<Geometry::Vec3f, kNumberOfVerticesInCube>& corners) {
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

} // namespace Interface
