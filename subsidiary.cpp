#include "subsidiary.hpp"

#include <iostream>
#include <vector>
#include <optional>

std::optional<std::vector<Triangle>> ReadTriangles(std::istream& in) {
    size_t size = 0;
    if (!(in >> size)) {
        std::cerr << "Failed to read size\n";
        return std::nullopt;
    }

    std::vector<Triangle> triangles;
    triangles.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        float line[9] {};
        for (float& value : line) {
            if (!(in >> value)) {
                std::cerr << "Failed to read triangle #" + std::to_string(i);
                return std::nullopt;
            }
        }

        triangles.emplace_back(Vec3(line[0], line[1], line[2]),
                               Vec3(line[3], line[4], line[5]),
                               Vec3(line[6], line[7], line[8]));
    }

    return triangles;
}

std::vector<float> FlattenVertices(const std::vector<Triangle>& triangles, const std::vector<bool>& intersecting) {
    std::vector<float> flat;
    flat.reserve(triangles.size() * 12);

    for (size_t tr_index = 0; tr_index < triangles.size(); tr_index++) {
        const Triangle& tr = triangles[tr_index];

        for (size_t i = 0; i < 3; i++) {
            const Vec3& vertex = tr.GetVertex(i);
            flat.push_back(vertex.GetX());
            flat.push_back(vertex.GetY());
            flat.push_back(vertex.GetZ());
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
            0, 0, -1, 0};
}

Matrix4x4 ViewMatrix(const Vec3& eye, const Vec3& target, const Vec3& up) {
    Vec3 direction_unnormalized = eye - target;
    Vec3 direction = direction_unnormalized.Normalize();

    Vec3 right = up.FindCross(direction).Normalize(); // господи как это ужасно ;;;(((
    Vec3 up_real = direction.FindCross(right);

    return {right.GetX(), right.GetY(), right.GetZ(), -right.FindDot(eye),
            up_real.GetX(), up_real.GetY(), up_real.GetZ(), -up_real.FindDot(eye),
            direction.GetX(), direction.GetY(), direction.GetZ(), -direction.FindDot(eye),
            0, 0, 0, 1};
}
