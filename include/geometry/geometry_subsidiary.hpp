#ifndef GEOMETRY_SUBSIDIARY_HPP_
#define GEOMETRY_SUBSIDIARY_HPP_

#include "triangle.hpp"
#include "vec3.hpp"

namespace Geometry {

template <typename T>
std::pair<Vec3<T>, Vec3<T>> FindBoundingBox(const Triangle<T>& triangle) {
    constexpr T kInf = std::numeric_limits<T>::infinity();
    std::array<T, 3> min_coord = { kInf, kInf, kInf };
    std::array<T, 3> max_coord = { -kInf, -kInf, -kInf };

    for (size_t i = 0; i < 3; i++) {
        const Vec3<T>& v = triangle.GetVertex(i);
        const std::array<T, 3> coords = { v.x_, v.y_, v.z_ };
        for (size_t axis = 0; axis < 3; axis++) {
            min_coord[axis] = std::min(min_coord[axis], coords[axis]);
            max_coord[axis] = std::max(max_coord[axis], coords[axis]);
        }
    }

    return { Vec3<T>(min_coord[0], min_coord[1], min_coord[2]),
             Vec3<T>(max_coord[0], max_coord[1], max_coord[2]) };
}

template <typename T>
std::pair<Vec3<T>, Vec3<T>> FindBoundingBox(const std::vector<Triangle<T>>& triangles) {
    constexpr T kInf = std::numeric_limits<T>::infinity();
    std::array<T, 3> min_coord = { kInf, kInf, kInf };
    std::array<T, 3> max_coord = { -kInf, -kInf, -kInf };

    for (const Triangle<T>& triangle : triangles) {
        const auto [box_min, box_max] = FindBoundingBox(triangle);
        const std::array<T, 3> low = { box_min.x_, box_min.y_, box_min.z_ };
        const std::array<T, 3> high = { box_max.x_, box_max.y_, box_max.z_ };
        for (size_t axis = 0; axis < 3; axis++) {
            min_coord[axis] = std::min(min_coord[axis], low[axis]);
            max_coord[axis] = std::max(max_coord[axis], high[axis]);
        }
    }

    return { Vec3<T>(min_coord[0], min_coord[1], min_coord[2]),
             Vec3<T>(max_coord[0], max_coord[1], max_coord[2]) };
}

} // namespace Geometry

#endif // GEOMETRY_SUBSIDIARY_HPP_
