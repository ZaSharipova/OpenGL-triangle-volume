#include "geometry_subsidiary.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <optional>
#include <numbers>

namespace Geometry {

bool AreEqual(const float number1, const float number2) {
    return std::abs(number1 - number2) < kEps;
}

bool IsZero(float vec) {
    return AreEqual(vec, 0.0);
}

bool IsZero(const Vec3& vec) {
    return Dot(vec, vec) <= kEps * kEps;
}

std::pair<Vec3, Vec3> FindBoundingBox(const Triangle& triangle) {
    constexpr float kInf = std::numeric_limits<float>::infinity();
    std::array<float, 3> min_coord = {kInf, kInf, kInf};
    std::array<float, 3> max_coord = {-kInf, -kInf, -kInf};

    for (size_t i = 0; i < 3; i++) {
        const Vec3& v = triangle.GetVertex(i);
        const std::array<float, 3> coords = {v.GetX(), v.GetY(), v.GetZ()};
        for (size_t axis = 0; axis < 3; axis++) {
            min_coord[axis] = std::min(min_coord[axis], coords[axis]);
            max_coord[axis] = std::max(max_coord[axis], coords[axis]);
        }
    }

    return {Vec3(min_coord[0], min_coord[1], min_coord[2]),
            Vec3(max_coord[0], max_coord[1], max_coord[2])};
}

std::pair<Vec3, Vec3> FindBoundingBox(const std::vector<Triangle>& triangles) {
    constexpr float kInf = std::numeric_limits<float>::infinity();
    std::array<float, 3> min_coord = {kInf, kInf, kInf};
    std::array<float, 3> max_coord = {-kInf, -kInf, -kInf};

    for (const Triangle& triangle : triangles) {
        const auto [box_min, box_max] = FindBoundingBox(triangle);
        const std::array<float, 3> low = {box_min.GetX(), box_min.GetY(), box_min.GetZ()};
        const std::array<float, 3> high = {box_max.GetX(), box_max.GetY(), box_max.GetZ()};
        for (size_t axis = 0; axis < 3; axis++) {
            min_coord[axis] = std::min(min_coord[axis], low[axis]);
            max_coord[axis] = std::max(max_coord[axis], high[axis]);
        }
    }

    return {Vec3(min_coord[0], min_coord[1], min_coord[2]),
            Vec3(max_coord[0], max_coord[1], max_coord[2])};
}

} // namespace Geometry
