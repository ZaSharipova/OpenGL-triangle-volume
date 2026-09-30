#include <gtest/gtest.h>

#include "triangle.hpp"

namespace {

geometry::Triangle MakeTriangle(const geometry::Vec3& a, const geometry::Vec3& b, const geometry::Vec3& c) {
    return geometry::Triangle(a, b, c);
}

const geometry::Triangle kOriginRightTriangleSmall =
    MakeTriangle({0, 0, 0}, {2, 0, 0}, {0, 2, 0});

const geometry::Triangle kOriginRightTriangleMedium =
    MakeTriangle({0, 0, 0}, {3, 0, 0}, {0, 3, 0});

const geometry::Triangle kOriginRightTriangleLarge =
    MakeTriangle({0, 0, 0}, {4, 0, 0}, {0, 4, 0});

}  // namespace

TEST(HaveIntersection, CoplanarOverlappingTrianglesIntersect) {
    geometry::Triangle b = MakeTriangle({1, 1, 0}, {5, 1, 0}, {1, 5, 0});

    EXPECT_TRUE(kOriginRightTriangleLarge.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleLarge));
}

TEST(HaveIntersection, CoplanarSeparatedTrianglesDoNotIntersect) {
    geometry::Triangle a = MakeTriangle({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
    geometry::Triangle b = MakeTriangle({10, 10, 0}, {11, 10, 0}, {10, 11, 0});

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TEST(HaveIntersection, CoplanarTrianglesTouchingAtVertexNotIntersect) {
    geometry::Triangle b = MakeTriangle({2, 2, 0}, {4, 2, 0}, {2, 4, 0});

    EXPECT_FALSE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, CoplanarTrianglesTouchingAlongEdgeIntersect) {
    geometry::Triangle b = MakeTriangle({2, 0, 0}, {0, 2, 0}, {2, 2, 0});

    EXPECT_TRUE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, CoplanarTriangleFullyInsideAnotherIntersects) {
    geometry::Triangle outer = MakeTriangle({0, 0, 0}, {10, 0, 0}, {0, 10, 0});
    geometry::Triangle inner = MakeTriangle({1, 1, 0}, {3, 1, 0}, {1, 3, 0});

    EXPECT_TRUE(outer.HaveIntersection(inner));
    EXPECT_TRUE(inner.HaveIntersection(outer));
}

TEST(HaveIntersection, IdenticalTrianglesIntersect) {
    EXPECT_TRUE(
        kOriginRightTriangleMedium.HaveIntersection(kOriginRightTriangleMedium));
}

TEST(HaveIntersection, CoplanarNearMissDoesNotIntersect) {
    geometry::Triangle b = MakeTriangle({2.1f, 0, 0}, {0, 2.1f, 0}, {2.1f, 2.1f, 0});

    EXPECT_FALSE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, PerpendicularTrianglesPiercingEachOtherIntersect) {
    geometry::Triangle a = MakeTriangle({-5, -5, 0}, {5, -5, 0}, {0, 5, 0});
    geometry::Triangle b = MakeTriangle({0, 0, -5}, {0, 0, 5}, {0, 3, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, ParallelPlanesDoNotIntersect) {
    geometry::Triangle b = MakeTriangle({0, 0, 5}, {4, 0, 5}, {0, 4, 5});

    EXPECT_FALSE(kOriginRightTriangleLarge.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleLarge));
}

TEST(HaveIntersection, ParallelPlanesDoNotIntersectSecond) {
    geometry::Triangle a = MakeTriangle({0, 0, 0}, {1, 0, 0}, {2, 0, 0});
    geometry::Triangle b = MakeTriangle({0, 1, 0}, {1, 1, 0}, {2, 1, 0});

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TEST(HaveIntersection, NonCoplanarTrianglesSeparatedByPlaneDoNotIntersect) {
    geometry::Triangle a = MakeTriangle({0, 0, -1}, {3, 0, 0}, {0, 3, -2});
    geometry::Triangle b = MakeTriangle({0, 0, 3}, {3, 0, 4}, {0, 3, 5});

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TEST(HaveIntersection, NonCoplanarTrianglesTouchingAtSinglePointIntersect) {
    geometry::Triangle b = MakeTriangle({0, 0, 0}, {0, 2, 2}, {0, -2, 2});

    EXPECT_TRUE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, TwoDegenerateTrianglesOverlappingSegmentsIntersect) {
    geometry::Triangle a = MakeTriangle({0, 0, 0}, {1, 0, 0}, {2, 0, 0});
    geometry::Triangle b = MakeTriangle({1.5f, 0, 0}, {2.5f, 0, 0}, {3.5f, 0, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
}

TEST(HaveIntersection, OverlapWithoutSharedVertices) {
    geometry::Triangle a = MakeTriangle({0, 0, 0}, {4, 0, 0}, {2, 3, 0});
    geometry::Triangle b = MakeTriangle({0, 2, 0}, {4, 2, 0}, {2, -1, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, DetectsVertexLyingOnEdge) {
    geometry::Triangle b = MakeTriangle({1, 1, 0}, {3, 1, 0}, {1, 3, 0});

    EXPECT_TRUE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, SegmentCrossesThroughTriangle) {
    geometry::Triangle b = MakeTriangle({0, 0, 0}, {4, 0, 0}, {0, 4, 0});
    geometry::Triangle a = MakeTriangle({-1, 2, 0}, {5, 2, 0}, {2, 2, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, SeparatedSegmentsDoNotTouch) {
    geometry::Triangle a = MakeTriangle({0, 0, 0}, {1, 0, 0}, {2, 0, 0});
    geometry::Triangle b = MakeTriangle({5, 0, 0}, {6, 0, 0}, {7, 0, 0});

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TEST(HaveIntersection, MatchesIdenticalSegments) {
    geometry::Triangle a = MakeTriangle({0, 0, 0}, {2, 0, 0}, {4, 0, 0});

    EXPECT_TRUE(a.HaveIntersection(a));
}

TEST(HaveIntersection, SegmentPiercesFace) {
    geometry::Triangle b = MakeTriangle({-2, -2, 0}, {2, -2, 0}, {0, 2, 0});
    geometry::Triangle a = MakeTriangle({0, 0, -3}, {0, 0, 3}, {0, 0, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, ApexTouchesFaceInterior) {
    geometry::Triangle b = MakeTriangle({-3, -3, 0}, {3, -3, 0}, {0, 4, 0});
    geometry::Triangle a = MakeTriangle({0, 0, 0}, {1, 1, 3}, {-1, 1, 3});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, DetectsThinSliverOverlap) {
    geometry::Triangle big = MakeTriangle({0, 0, 0}, {10, 0, 0}, {0, 10, 0});
    geometry::Triangle sliver = MakeTriangle({-1, 2, 0}, {11, 2, 0}, {5, 2.01f, 0});

    EXPECT_TRUE(big.HaveIntersection(sliver));
    EXPECT_TRUE(sliver.HaveIntersection(big));
}
