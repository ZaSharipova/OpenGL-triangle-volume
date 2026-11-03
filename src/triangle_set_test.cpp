#include <gtest/gtest.h>

#include <cstdlib>
#include <vector>

#include "config.hpp"
#include "triangle_set.hpp"

namespace {

std::vector<bool> FindIntersectingFlagsBruteForce(const std::vector<Geometry::Trianglef>& triangles) {
    std::vector<bool> intersecting(triangles.size(), false);
    for (size_t i = 0; i < triangles.size(); i++) {
        for (size_t j = i + 1; j < triangles.size(); j++) {
            if (triangles[i].HaveIntersection(triangles[j])) {
                intersecting[i] = intersecting[j] = true;
            }
        }
    }
    return intersecting;
}

float RandomFloat(float low, float high) {
    return low + (high - low) * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

std::vector<Geometry::Trianglef> GenerateTriangles(size_t count, float scene_size, float triangle_size) {
    std::vector<Geometry::Trianglef> triangles;

    for (size_t i = 0; i < count; i++) {
        const float x = RandomFloat(0, scene_size);
        const float y = RandomFloat(0, scene_size);
        const float z = RandomFloat(0, scene_size);

        Geometry::Vec3f a(x + RandomFloat(-triangle_size, triangle_size),
            y + RandomFloat(-triangle_size, triangle_size),
            z + RandomFloat(-triangle_size, triangle_size));
        Geometry::Vec3f b(x + RandomFloat(-triangle_size, triangle_size),
            y + RandomFloat(-triangle_size, triangle_size),
            z + RandomFloat(-triangle_size, triangle_size));
        Geometry::Vec3f c(x + RandomFloat(-triangle_size, triangle_size),
            y + RandomFloat(-triangle_size, triangle_size),
            z + RandomFloat(-triangle_size, triangle_size));

        triangles.emplace_back(a, b, c);
    }

    return triangles;
}

} // namespace

TEST(FindIntersectingFlags, GridMatchesBruteForce) {
    for (unsigned seed = 0; seed < 100; seed++) {
        std::srand(seed);
        for (size_t count : { 2, 10, 50, 200, 500 }) {
            const std::vector<Geometry::Trianglef> triangles = GenerateTriangles(count, 10, 2);

            ASSERT_EQ(FindIntersectingFlags(triangles), FindIntersectingFlagsBruteForce(triangles))
                << "seed = " << seed << ", count = " << count;
        }
    }
}
