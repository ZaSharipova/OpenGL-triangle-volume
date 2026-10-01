#include "geometry_subsidiary.hpp"
#include "subsidiary.hpp"
#include "triangle.hpp"
#include "input.hpp"

#include <sstream>
#include <vector>
#include <list>
#include <gtest/gtest.h>


TEST(EndToEnd, NoIntersections) {
    std::istringstream input(R"(
        2
        0 0 0 1 0 0 2 0 0
        0 1 0 1 1 0 2 1 0)");

    std::optional<std::vector<geometry::Triangle>> triangles = ReadTriangles(input);

    EXPECT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({false, false}));
}

TEST(EndToEnd, TwoIntersecting) {
    std::istringstream input(R"(
        2
        0 0 0 2 0 0 0 2 0
        0.5 0.5 -1 0.5 0.5 1 1 1 1\n)");

    std::optional<std::vector<geometry::Triangle>> triangles = ReadTriangles(input);

    EXPECT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({true, true}));
}

TEST(EndToEnd, OneFarAway) {
    std::istringstream input(R"(
    3
    0 0 0 2 0 0 0 2 0
    0.5 0.5 -1 0.5 0.5 1 1 1 1
    10 10 10 11 10 10 10 11 10)");

    std::optional<std::vector<geometry::Triangle>> triangles = ReadTriangles(input);

    EXPECT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({true, true, false}));
}
