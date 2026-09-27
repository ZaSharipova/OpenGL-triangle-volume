#include <gtest/gtest.h>

#include "triangle.hpp"

namespace {

Triangle MakeTriangle(const Vec3& a, const Vec3& b, const Vec3& c) {
    return Triangle(a, b, c);
}

const Triangle kOriginRightTriangleSmall =
    MakeTriangle({0, 0, 0}, {2, 0, 0}, {0, 2, 0});

const Triangle kOriginRightTriangleMedium =
    MakeTriangle({0, 0, 0}, {3, 0, 0}, {0, 3, 0});

const Triangle kOriginRightTriangleLarge =
    MakeTriangle({0, 0, 0}, {4, 0, 0}, {0, 4, 0});

}  // namespace

TEST(HaveIntersection, CoplanarOverlappingTrianglesIntersect) {
    Triangle b = MakeTriangle({1, 1, 0}, {5, 1, 0}, {1, 5, 0});

    EXPECT_TRUE(kOriginRightTriangleLarge.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleLarge));
}

TEST(HaveIntersection, CoplanarSeparatedTrianglesDoNotIntersect) {
    Triangle a = MakeTriangle({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
    Triangle b = MakeTriangle({10, 10, 0}, {11, 10, 0}, {10, 11, 0});

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TEST(HaveIntersection, CoplanarTrianglesTouchingAtVertexNotIntersect) {
    Triangle b = MakeTriangle({2, 2, 0}, {4, 2, 0}, {2, 4, 0});

    EXPECT_FALSE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, CoplanarTrianglesTouchingAlongEdgeIntersect) {
    Triangle b = MakeTriangle({2, 0, 0}, {0, 2, 0}, {2, 2, 0});

    EXPECT_TRUE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, CoplanarTriangleFullyInsideAnotherIntersects) {
    Triangle outer = MakeTriangle({0, 0, 0}, {10, 0, 0}, {0, 10, 0});
    Triangle inner = MakeTriangle({1, 1, 0}, {3, 1, 0}, {1, 3, 0});

    EXPECT_TRUE(outer.HaveIntersection(inner));
    EXPECT_TRUE(inner.HaveIntersection(outer));
}

TEST(HaveIntersection, IdenticalTrianglesIntersect) {
    EXPECT_TRUE(
        kOriginRightTriangleMedium.HaveIntersection(kOriginRightTriangleMedium));
}

TEST(HaveIntersection, CoplanarNearMissDoesNotIntersect) {
    Triangle b = MakeTriangle({2.1f, 0, 0}, {0, 2.1f, 0}, {2.1f, 2.1f, 0});

    EXPECT_FALSE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, PerpendicularTrianglesPiercingEachOtherIntersect) {
    Triangle a = MakeTriangle({-5, -5, 0}, {5, -5, 0}, {0, 5, 0});
    Triangle b = MakeTriangle({0, 0, -5}, {0, 0, 5}, {0, 3, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, ParallelPlanesDoNotIntersect) {
    Triangle b = MakeTriangle({0, 0, 5}, {4, 0, 5}, {0, 4, 5});

    EXPECT_FALSE(kOriginRightTriangleLarge.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleLarge));
}

TEST(HaveIntersection, NonCoplanarTrianglesSeparatedByPlaneDoNotIntersect) {
    Triangle a = MakeTriangle({0, 0, -1}, {3, 0, 0}, {0, 3, -2});
    Triangle b = MakeTriangle({0, 0, 3}, {3, 0, 4}, {0, 3, 5});

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TEST(HaveIntersection, NonCoplanarTrianglesTouchingAtSinglePointIntersect) {
    Triangle b = MakeTriangle({0, 0, 0}, {0, 2, 2}, {0, -2, 2});

    EXPECT_TRUE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, TwoDegenerateTrianglesOverlappingSegmentsIntersect) {
    Triangle a = MakeTriangle({0, 0, 0}, {1, 0, 0}, {2, 0, 0});
    Triangle b = MakeTriangle({1.5f, 0, 0}, {2.5f, 0, 0}, {3.5f, 0, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
}
