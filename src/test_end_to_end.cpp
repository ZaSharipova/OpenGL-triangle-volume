#include "config.hpp"
#include "geometry_subsidiary.hpp"
#include "input.hpp"
#include "interface/subsidiary.hpp"
#include "triangle_set.hpp"

#include <gtest/gtest.h>
#include <list>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace {

template <typename T>
std::optional<std::vector<Geometry::Triangle<T>>> ReadTriangles(const std::string& text) {
    std::istringstream input(text);
    return Interface::Input::ReadTriangles<T>(input);
}

} // namespace

template <typename T>
class EndToEnd : public ::testing::Test {};

using ScalarTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(EndToEnd, ScalarTypes);

TYPED_TEST(EndToEnd, NoIntersections) {
    auto triangles = ReadTriangles<TypeParam>(R"(
        2
        0 0 0 1 0 0 2 0 0
        0 1 0 1 1 0 2 1 0)");

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({ false, false }));
}

TYPED_TEST(EndToEnd, TwoIntersecting) {
    auto triangles = ReadTriangles<TypeParam>(R"(
        2
        0 0 0 2 0 0 0 2 0
        0.5 0.5 -1 0.5 0.5 1 1 1 1)");

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({ true, true }));
}

TYPED_TEST(EndToEnd, OneFarAway) {
    auto triangles = ReadTriangles<TypeParam>(R"(
        3
        0 0 0 2 0 0 0 2 0
        0.5 0.5 -1 0.5 0.5 1 1 1 1
        10 10 10 11 10 10 10 11 10)");

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({ true, true, false }));
}

TYPED_TEST(EndToEnd, NonNumericInputReturnsNullopt) {
    EXPECT_FALSE(ReadTriangles<TypeParam>(R"(
        1
        0 0 0 1 0 0 abc 0 0)").has_value());
}

TYPED_TEST(EndToEnd, NotEnoughCoordinatesReturnsNullopt) {
    EXPECT_FALSE(ReadTriangles<TypeParam>(R"(
        1
        0 0 0 1 0 0 2 0)").has_value());
}

TYPED_TEST(EndToEnd, FewerTrianglesThanDeclaredReturnsNullopt) {
    EXPECT_FALSE(ReadTriangles<TypeParam>(R"(
        3
        0 0 0 1 0 0 0 1 0
        5 5 5 6 5 5 5 6 5)").has_value());
}

TYPED_TEST(EndToEnd, EmptyInputReturnsNullopt) {
    EXPECT_FALSE(ReadTriangles<TypeParam>("").has_value());
}

TYPED_TEST(EndToEnd, NonNumericCountReturnsNullopt) {
    EXPECT_FALSE(ReadTriangles<TypeParam>(R"(
        two
        0 0 0 1 0 0 0 1 0)").has_value());
}

TYPED_TEST(EndToEnd, CoordinateOrderIsXYZPerVertex) {
    auto triangles = ReadTriangles<TypeParam>(R"(
        3
        0 0 0 4 0 0 0 4 0
        1 1 0 2 1 0 1 2 0
        1 1 3 2 1 3 1 2 3)");

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>({ true, true, false }));
}

TYPED_TEST(EndToEnd, MixedFlags) {
    auto triangles = ReadTriangles<TypeParam>(R"(
        5
        0 0 0 4 0 0 0 4 0
        1 1 -1 1 1 1 2 1 1
        50 50 50 51 50 50 50 51 50
        100 0 0 104 0 0 100 4 0
        101 1 0 105 1 0 101 5 0)");

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()),
        std::vector<bool>({ true, true, false, true, true }));
}

TYPED_TEST(EndToEnd, ManyGroupsExpectedFlags) {
    const int groups = 1000;
    std::ostringstream text;
    text << groups * 3 << "\n";
    std::vector<bool> expected;

    for (int k = 0; k < groups; k++) {
        const int x = 100 * k;
        text << x << " 0 0 " << x + 4 << " 0 0 " << x << " 4 0\n";
        text << x + 1 << " 1 0 " << x + 5 << " 1 0 " << x + 1 << " 5 0\n";
        text << x + 50 << " 0 0 " << x + 52 << " 0 0 " << x + 50 << " 2 0\n";
        expected.insert(expected.end(), { true, true, false });
    }

    auto triangles = ReadTriangles<TypeParam>(text.str());

    ASSERT_TRUE(triangles.has_value());
    ASSERT_EQ(triangles->size(), expected.size());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), expected);
}

TYPED_TEST(EndToEnd, ManyFarApartTrianglesNoFlags) {
    const int count = 5000;
    std::ostringstream text;
    text << count << "\n";
    for (int i = 0; i < count; i++) {
        const int x = 10 * i;
        text << x << " 0 0 " << x + 2 << " 0 0 " << x << " 2 0\n";
    }

    auto triangles = ReadTriangles<TypeParam>(text.str());

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>(count, false));
}

TYPED_TEST(EndToEnd, ManyIdenticalTrianglesAllFlagged) {
    const int count = 2000;
    std::ostringstream text;
    text << count << "\n";
    for (int i = 0; i < count; i++) {
        text << "0 0 0 3 0 0 0 3 0\n";
    }

    auto triangles = ReadTriangles<TypeParam>(text.str());

    ASSERT_TRUE(triangles.has_value());
    EXPECT_EQ(FindIntersectingFlags(triangles.value()), std::vector<bool>(count, true));
}

TYPED_TEST(EndToEnd, LargeInputWithMissingTriangleReturnsNullopt) {
    const int count = 5000;
    std::ostringstream text;
    text << count + 1 << "\n";
    for (int i = 0; i < count; i++) {
        const int x = 10 * i;
        text << x << " 0 0 " << x + 2 << " 0 0 " << x << " 2 0\n";
    }

    EXPECT_FALSE(ReadTriangles<TypeParam>(text.str()).has_value());
}
