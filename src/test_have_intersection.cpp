#include <gtest/gtest.h>

#include "triangle.hpp"

namespace {

Geometry::Triangle MakeTriangle(const Geometry::Vec3& a, const Geometry::Vec3& b, const Geometry::Vec3& c) {
    return Geometry::Triangle(a, b, c);
}

const Geometry::Triangle kOriginRightTriangleSmall =
    MakeTriangle({0, 0, 0}, {2, 0, 0}, {0, 2, 0});

const Geometry::Triangle kOriginRightTriangleMedium =
    MakeTriangle({0, 0, 0}, {3, 0, 0}, {0, 3, 0});

const Geometry::Triangle kOriginRightTriangleLarge =
    MakeTriangle({0, 0, 0}, {4, 0, 0}, {0, 4, 0});

}  // namespace

TEST(HaveIntersection, CoplanarOverlappingTrianglesIntersect) {
    Geometry::Triangle b = MakeTriangle({1, 1, 0}, {5, 1, 0}, {1, 5, 0});

    EXPECT_TRUE(kOriginRightTriangleLarge.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleLarge));
}

TEST(HaveIntersection, CoplanarSeparatedTrianglesDoNotIntersect) {
    Geometry::Triangle a = MakeTriangle({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
    Geometry::Triangle b = MakeTriangle({10, 10, 0}, {11, 10, 0}, {10, 11, 0});

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TEST(HaveIntersection, CoplanarTrianglesTouchingAtVertexNotIntersect) {
    Geometry::Triangle b = MakeTriangle({2, 2, 0}, {4, 2, 0}, {2, 4, 0});

    EXPECT_FALSE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, CoplanarTrianglesTouchingAlongEdgeIntersect) {
    Geometry::Triangle b = MakeTriangle({2, 0, 0}, {0, 2, 0}, {2, 2, 0});

    EXPECT_TRUE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, CoplanarTriangleFullyInsideAnotherIntersects) {
    Geometry::Triangle outer = MakeTriangle({0, 0, 0}, {10, 0, 0}, {0, 10, 0});
    Geometry::Triangle inner = MakeTriangle({1, 1, 0}, {3, 1, 0}, {1, 3, 0});

    EXPECT_TRUE(outer.HaveIntersection(inner));
    EXPECT_TRUE(inner.HaveIntersection(outer));
}

TEST(HaveIntersection, IdenticalTrianglesIntersect) {
    EXPECT_TRUE(
        kOriginRightTriangleMedium.HaveIntersection(kOriginRightTriangleMedium));
}

TEST(HaveIntersection, CoplanarNearMissDoesNotIntersect) {
    Geometry::Triangle b = MakeTriangle({2.1f, 0, 0}, {0, 2.1f, 0}, {2.1f, 2.1f, 0});

    EXPECT_FALSE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, PerpendicularTrianglesPiercingEachOtherIntersect) {
    Geometry::Triangle a = MakeTriangle({-5, -5, 0}, {5, -5, 0}, {0, 5, 0});
    Geometry::Triangle b = MakeTriangle({0, 0, -5}, {0, 0, 5}, {0, 3, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, ParallelPlanesDoNotIntersect) {
    Geometry::Triangle b = MakeTriangle({0, 0, 5}, {4, 0, 5}, {0, 4, 5});

    EXPECT_FALSE(kOriginRightTriangleLarge.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleLarge));
}

TEST(HaveIntersection, ParallelPlanesDoNotIntersectSecond) {
    Geometry::Triangle a = MakeTriangle({0, 0, 0}, {1, 0, 0}, {2, 0, 0});
    Geometry::Triangle b = MakeTriangle({0, 1, 0}, {1, 1, 0}, {2, 1, 0});

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TEST(HaveIntersection, NonCoplanarTrianglesSeparatedByPlaneDoNotIntersect) {
    Geometry::Triangle a = MakeTriangle({0, 0, -1}, {3, 0, 0}, {0, 3, -2});
    Geometry::Triangle b = MakeTriangle({0, 0, 3}, {3, 0, 4}, {0, 3, 5});

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TEST(HaveIntersection, NonCoplanarTrianglesTouchingAtSinglePointIntersect) {
    Geometry::Triangle b = MakeTriangle({0, 0, 0}, {0, 2, 2}, {0, -2, 2});

    EXPECT_TRUE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, TwoDegenerateTrianglesOverlappingSegmentsIntersect) {
    Geometry::Triangle a = MakeTriangle({0, 0, 0}, {1, 0, 0}, {2, 0, 0});
    Geometry::Triangle b = MakeTriangle({1.5f, 0, 0}, {2.5f, 0, 0}, {3.5f, 0, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
}

TEST(HaveIntersection, OverlapWithoutSharedVertices) {
    Geometry::Triangle a = MakeTriangle({0, 0, 0}, {4, 0, 0}, {2, 3, 0});
    Geometry::Triangle b = MakeTriangle({0, 2, 0}, {4, 2, 0}, {2, -1, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, DetectsVertexLyingOnEdge) {
    Geometry::Triangle b = MakeTriangle({1, 1, 0}, {3, 1, 0}, {1, 3, 0});

    EXPECT_TRUE(kOriginRightTriangleSmall.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall));
}

TEST(HaveIntersection, SegmentCrossesThroughTriangle) {
    Geometry::Triangle b = MakeTriangle({0, 0, 0}, {4, 0, 0}, {0, 4, 0});
    Geometry::Triangle a = MakeTriangle({-1, 2, 0}, {5, 2, 0}, {2, 2, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, SeparatedSegmentsDoNotTouch) {
    Geometry::Triangle a = MakeTriangle({0, 0, 0}, {1, 0, 0}, {2, 0, 0});
    Geometry::Triangle b = MakeTriangle({5, 0, 0}, {6, 0, 0}, {7, 0, 0});

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TEST(HaveIntersection, MatchesIdenticalSegments) {
    Geometry::Triangle a = MakeTriangle({0, 0, 0}, {2, 0, 0}, {4, 0, 0});

    EXPECT_TRUE(a.HaveIntersection(a));
}

TEST(HaveIntersection, SegmentPiercesFace) {
    Geometry::Triangle b = MakeTriangle({-2, -2, 0}, {2, -2, 0}, {0, 2, 0});
    Geometry::Triangle a = MakeTriangle({0, 0, -3}, {0, 0, 3}, {0, 0, 0});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, ApexTouchesFaceInterior) {
    Geometry::Triangle b = MakeTriangle({-3, -3, 0}, {3, -3, 0}, {0, 4, 0});
    Geometry::Triangle a = MakeTriangle({0, 0, 0}, {1, 1, 3}, {-1, 1, 3});

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TEST(HaveIntersection, DetectsThinSliverOverlap) {
    Geometry::Triangle big = MakeTriangle({0, 0, 0}, {10, 0, 0}, {0, 10, 0});
    Geometry::Triangle sliver = MakeTriangle({-1, 2, 0}, {11, 2, 0}, {5, 2.01f, 0});

    EXPECT_TRUE(big.HaveIntersection(sliver));
    EXPECT_TRUE(sliver.HaveIntersection(big));
}

TEST(HaveIntersectionMany, TrianglesSharingOneVertexAllIntersect) {
    std::vector<Geometry::Triangle> ts;
    for (int i = 1; i <= 300; i++) {
        float f = static_cast<float>(i);
        ts.push_back(MakeTriangle({0, 0, 0}, {1, f, 1}, {f, 1, -1}));
    }

    for (size_t i = 0; i < ts.size(); i++) {
        for (size_t j = i + 1; j < ts.size(); j++) {
            EXPECT_TRUE(ts[i].HaveIntersection(ts[j])) << i << " " << j;
            EXPECT_TRUE(ts[j].HaveIntersection(ts[i])) << j << " " << i;
        }
    }
}

TEST(HaveIntersectionMany, TrianglesSharingOneEdgeAllIntersect) {
    std::vector<Geometry::Triangle> ts;
    for (int i = 1; i <= 300; i++) {
        float f = static_cast<float>(i);
        ts.push_back(MakeTriangle({0, 0, -1}, {0, 0, 1}, {f, 1, 0}));
    }

    for (size_t i = 0; i < ts.size(); i++) {
        for (size_t j = i + 1; j < ts.size(); j++) {
            EXPECT_TRUE(ts[i].HaveIntersection(ts[j])) << i << " " << j;
            EXPECT_TRUE(ts[j].HaveIntersection(ts[i])) << j << " " << i;
        }
    }
}

TEST(HaveIntersectionMany, NestedCoplanarTrianglesAllIntersect) {
    std::vector<Geometry::Triangle> ts;
    for (int i = 1; i <= 300; i++) {
        float f = static_cast<float>(i);
        ts.push_back(MakeTriangle({-f, -f, 0}, {f, -f, 0}, {0, f, 0}));
    }

    for (size_t i = 0; i < ts.size(); i++) {
        for (size_t j = i + 1; j < ts.size(); j++) {
            EXPECT_TRUE(ts[i].HaveIntersection(ts[j])) << i << " " << j;
            EXPECT_TRUE(ts[j].HaveIntersection(ts[i])) << j << " " << i;
        }
    }
}

TEST(HaveIntersectionMany, ChainTouchingAtVerticesIntersectsOnlyNeighbours) {
    std::vector<Geometry::Triangle> ts;
    for (int i = 0; i < 300; i++) {
        float x = 2.0f * static_cast<float>(i);
        ts.push_back(MakeTriangle({x, 0, 0}, {x + 2, 0, 0}, {x, 2, 0}));
    }

    for (size_t i = 0; i < ts.size(); i++) {
        for (size_t j = i + 1; j < ts.size(); j++) {
            bool expected = (j - i == 1);
            EXPECT_EQ(ts[i].HaveIntersection(ts[j]), expected) << i << " " << j;
            EXPECT_EQ(ts[j].HaveIntersection(ts[i]), expected) << j << " " << i;
        }
    }
}

TEST(HaveIntersectionMany, LongNeedlePiercesWholeStack) {
    const int n = 300;
    std::vector<Geometry::Triangle> stack;
    for (int i = 0; i < n; i++) {
        float z = static_cast<float>(i);
        stack.push_back(MakeTriangle({0, 0, z}, {4, 0, z}, {0, 4, z}));
    }
    Geometry::Triangle needle =
        MakeTriangle({0.5f, 0.5f, -1}, {0.5f, 0.5f, static_cast<float>(n)},
                     {0.75f, 0.5f, static_cast<float>(n)});

    for (size_t i = 0; i < stack.size(); i++) {
        EXPECT_TRUE(needle.HaveIntersection(stack[i])) << i;
        EXPECT_TRUE(stack[i].HaveIntersection(needle)) << i;
        for (size_t j = i + 1; j < stack.size(); j++) {
            EXPECT_FALSE(stack[i].HaveIntersection(stack[j])) << i << " " << j;
        }
    }
}
