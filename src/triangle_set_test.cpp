#include <gtest/gtest.h>

#include <cstdlib>
#include <vector>

#include "config.hpp"
#include "triangle_set.hpp"

namespace {

template <typename T>
std::vector<bool> FindIntersectingFlagsBruteForce(const std::vector<Geometry::Triangle<T>>& triangles) {
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

template <typename T>
T RandomFloatingPoint(T low, T high) {
    return low + (high - low) * static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX);
}

template <typename T>
std::vector<Geometry::Triangle<T>> GenerateTriangles(size_t count, T scene_size, T triangle_size) {
    std::vector<Geometry::Triangle<T>> triangles;

    for (size_t i = 0; i < count; i++) {
        const T x = RandomFloatingPoint<T>(0, scene_size);
        const T y = RandomFloatingPoint<T>(0, scene_size);
        const T z = RandomFloatingPoint<T>(0, scene_size);

        Geometry::Vec3<T> a(x + RandomFloatingPoint<T>(-triangle_size, triangle_size),
            y + RandomFloatingPoint<T>(-triangle_size, triangle_size),
            z + RandomFloatingPoint<T>(-triangle_size, triangle_size));
        Geometry::Vec3<T> b(x + RandomFloatingPoint(-triangle_size, triangle_size),
            y + RandomFloatingPoint(-triangle_size, triangle_size),
            z + RandomFloatingPoint(-triangle_size, triangle_size));
        Geometry::Vec3<T> c(x + RandomFloatingPoint(-triangle_size, triangle_size),
            y + RandomFloatingPoint(-triangle_size, triangle_size),
            z + RandomFloatingPoint(-triangle_size, triangle_size));

        triangles.emplace_back(a, b, c);
    }

    return triangles;
}

} // namespace

template <typename T>
class FindIntersectingFlagsTest : public ::testing::Test {};

using ScalarTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(FindIntersectingFlagsTest, ScalarTypes);

TYPED_TEST(FindIntersectingFlagsTest, GridMatchesBruteForce) {
    using T = TypeParam;

    for (unsigned seed = 0; seed < 100; seed++) {
        std::srand(seed);
        for (size_t count : { 2, 10, 50, 200, 500 }) {
            const std::vector<Geometry::Triangle<T>> triangles = GenerateTriangles<T>(count, 10, 2);

            ASSERT_EQ(FindIntersectingFlags(triangles), FindIntersectingFlagsBruteForce(triangles))
                << "seed = " << seed << ", count = " << count;
        }
    }
}
