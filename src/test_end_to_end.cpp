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

    std::optional<std::vector<Geometry::Triangle>> triangles = Input::ReadTriangles(input);

    EXPECT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({false, false}));
}

TEST(EndToEnd, TwoIntersecting) {
    std::istringstream input(R"(
        2
        0 0 0 2 0 0 0 2 0
        0.5 0.5 -1 0.5 0.5 1 1 1 1\n)");

    std::optional<std::vector<Geometry::Triangle>> triangles = Input::ReadTriangles(input);

    EXPECT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({true, true}));
}

TEST(EndToEnd, OneFarAway) {
    std::istringstream input(R"(
    3
    0 0 0 2 0 0 0 2 0
    0.5 0.5 -1 0.5 0.5 1 1 1 1
    10 10 10 11 10 10 10 11 10)");

    std::optional<std::vector<Geometry::Triangle>> triangles = Input::ReadTriangles(input);

    EXPECT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({true, true, false}));
}

#include <string>

TEST(EndToEnd, NonNumericInputReturnsNullopt) {
    std::istringstream input(R"(
        1
        0 0 0 1 0 0 abc 0 0)");

    EXPECT_FALSE(Input::ReadTriangles(input).has_value());
}

TEST(EndToEnd, NotEnoughCoordinatesReturnsNullopt) {
    std::istringstream input(R"(
        1
        0 0 0 1 0 0 2 0)");

    EXPECT_FALSE(Input::ReadTriangles(input).has_value());
}

TEST(EndToEnd, FewerTrianglesThanDeclaredReturnsNullopt) {
    std::istringstream input(R"(
        3
        0 0 0 1 0 0 0 1 0
        5 5 5 6 5 5 5 6 5)");

    EXPECT_FALSE(Input::ReadTriangles(input).has_value());
}

TEST(EndToEnd, EmptyInputReturnsNullopt) {
    std::istringstream input("");

    EXPECT_FALSE(Input::ReadTriangles(input).has_value());
}

TEST(EndToEnd, NonNumericCountReturnsNullopt) {
    std::istringstream input(R"(
        two
        0 0 0 1 0 0 0 1 0)");

    EXPECT_FALSE(Input::ReadTriangles(input).has_value());
}

TEST(EndToEnd, CoordinateOrderIsXYZPerVertex) {
    std::istringstream input(R"(
        3
        0 0 0 4 0 0 0 4 0
        1 1 0 2 1 0 1 2 0
        1 1 3 2 1 3 1 2 3)");

    std::optional<std::vector<Geometry::Triangle>> triangles = Input::ReadTriangles(input);

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({true, true, false}));
}

TEST(EndToEnd, MixedFlags) {
    std::istringstream input(R"(
        5
        0 0 0 4 0 0 0 4 0
        1 1 -1 1 1 1 2 1 1
        50 50 50 51 50 50 50 51 50
        100 0 0 104 0 0 100 4 0
        101 1 0 105 1 0 101 5 0)");

    std::optional<std::vector<Geometry::Triangle>> triangles = Input::ReadTriangles(input);

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()),
              std::vector<bool>({true, true, false, true, true}));
}

TEST(EndToEnd, ManyGroupsExpectedFlags) {
    const int groups = 1000;
    std::ostringstream text;
    text << groups * 3 << "\n";
    std::vector<bool> expected;

    for (int k = 0; k < groups; k++) {
        const int x = 100 * k;
        text << x << " 0 0 " << x + 4 << " 0 0 " << x << " 4 0\n";
        text << x + 1 << " 1 0 " << x + 5 << " 1 0 " << x + 1 << " 5 0\n";
        text << x + 50 << " 0 0 " << x + 52 << " 0 0 " << x + 50 << " 2 0\n";
        expected.insert(expected.end(), {true, true, false});
    }

    std::istringstream input(text.str());
    std::optional<std::vector<Geometry::Triangle>> triangles = Input::ReadTriangles(input);

    ASSERT_TRUE(triangles.has_value());
    ASSERT_EQ(triangles->size(), expected.size());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), expected);
}

TEST(EndToEnd, ManyFarApartTrianglesNoFlags) {
    const int count = 5000;
    std::ostringstream text;
    text << count << "\n";
    for (int i = 0; i < count; i++) {
        const int x = 10 * i;
        text << x << " 0 0 " << x + 2 << " 0 0 " << x << " 2 0\n";
    }

    std::istringstream input(text.str());
    std::optional<std::vector<Geometry::Triangle>> triangles = Input::ReadTriangles(input);

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>(count, false));
}

TEST(EndToEnd, ManyIdenticalTrianglesAllFlagged) {
    const int count = 2000;
    std::ostringstream text;
    text << count << "\n";
    for (int i = 0; i < count; i++) {
        text << "0 0 0 3 0 0 0 3 0\n";
    }

    std::istringstream input(text.str());
    std::optional<std::vector<Geometry::Triangle>> triangles = Input::ReadTriangles(input);

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>(count, true));
}

TEST(EndToEnd, LargeInputWithMissingTriangleReturnsNullopt) {
    const int count = 5000;
    std::ostringstream text;
    text << count + 1 << "\n";
    for (int i = 0; i < count; i++) {
        const int x = 10 * i;
        text << x << " 0 0 " << x + 2 << " 0 0 " << x << " 2 0\n";
    }

    std::istringstream input(text.str());

    EXPECT_FALSE((Input::ReadTriangles(input)).has_value());
}
