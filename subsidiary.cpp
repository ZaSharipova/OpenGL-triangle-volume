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

std::vector<float> FlattenVertices(const std::vector<Triangle>& triangles) {
    std::vector<float> flat;
    flat.reserve(triangles.size() * 9);

    for (const Triangle& tr : triangles) {
        for (size_t i = 0; i < 3; i++) {
            const Vec3& vertex = tr.GetVertex(i);
            flat.push_back(vertex.GetX());
            flat.push_back(vertex.GetY());
            flat.push_back(vertex.GetZ());
        }
    }

    return flat;
}
