#include <gtest/gtest.h>

#include <vector>

#include "config.hpp"
#include "triangle.hpp"

namespace {

template <typename T>
Geometry::Triangle<T> MakeTriangle(const Geometry::Vec3<T>& a, const Geometry::Vec3<T>& b, const Geometry::Vec3<T>& c) {
    return Geometry::Triangle<T>(a, b, c);
}

template <typename T>
const Geometry::Triangle<T> kOriginRightTriangleSmall = MakeTriangle<T>({ 0, 0, 0 }, { 2, 0, 0 }, { 0, 2, 0 });

template <typename T>
const Geometry::Triangle<T> kOriginRightTriangleMedium = MakeTriangle<T>({ 0, 0, 0 }, { 3, 0, 0 }, { 0, 3, 0 });

template <typename T>
const Geometry::Triangle<T> kOriginRightTriangleLarge = MakeTriangle<T>({ 0, 0, 0 }, { 4, 0, 0 }, { 0, 4, 0 });

} // namespace

template <typename T>
class HaveIntersection : public ::testing::Test {};

template <typename T>
class HaveIntersectionMany : public ::testing::Test {};

using ScalarTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(HaveIntersection, ScalarTypes);
TYPED_TEST_SUITE(HaveIntersectionMany, ScalarTypes);

TYPED_TEST(HaveIntersection, CoplanarOverlappingTrianglesIntersect) {
    using T = TypeParam;
    auto b = MakeTriangle<T>({ 1, 1, 0 }, { 5, 1, 0 }, { 1, 5, 0 });

    EXPECT_TRUE(kOriginRightTriangleLarge<T>.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleLarge<T>));
}

TYPED_TEST(HaveIntersection, CoplanarSeparatedTrianglesDoNotIntersect) {
    using T = TypeParam;
    auto a = MakeTriangle<T>({ 0, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 });
    auto b = MakeTriangle<T>({ 10, 10, 0 }, { 11, 10, 0 }, { 10, 11, 0 });

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TYPED_TEST(HaveIntersection, CoplanarTrianglesTouchingAtVertexNotIntersect) {
    using T = TypeParam;
    auto b = MakeTriangle<T>({ 2, 2, 0 }, { 4, 2, 0 }, { 2, 4, 0 });

    EXPECT_FALSE(kOriginRightTriangleSmall<T>.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleSmall<T>));
}

TYPED_TEST(HaveIntersection, CoplanarTrianglesTouchingAlongEdgeIntersect) {
    using T = TypeParam;
    auto b = MakeTriangle<T>({ 2, 0, 0 }, { 0, 2, 0 }, { 2, 2, 0 });

    EXPECT_TRUE(kOriginRightTriangleSmall<T>.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall<T>));
}

TYPED_TEST(HaveIntersection, CoplanarTriangleFullyInsideAnotherIntersects) {
    using T = TypeParam;
    auto outer = MakeTriangle<T>({ 0, 0, 0 }, { 10, 0, 0 }, { 0, 10, 0 });
    auto inner = MakeTriangle<T>({ 1, 1, 0 }, { 3, 1, 0 }, { 1, 3, 0 });

    EXPECT_TRUE(outer.HaveIntersection(inner));
    EXPECT_TRUE(inner.HaveIntersection(outer));
}

TYPED_TEST(HaveIntersection, IdenticalTrianglesIntersect) {
    using T = TypeParam;
    EXPECT_TRUE(kOriginRightTriangleMedium<T>.HaveIntersection(kOriginRightTriangleMedium<T>));
}

TYPED_TEST(HaveIntersection, CoplanarNearMissDoesNotIntersect) {
    using T = TypeParam;
    const T e = static_cast<T>(2.1);
    auto b = MakeTriangle<T>({ e, 0, 0 }, { 0, e, 0 }, { e, e, 0 });

    EXPECT_FALSE(kOriginRightTriangleSmall<T>.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleSmall<T>));
}

TYPED_TEST(HaveIntersection, PerpendicularTrianglesPiercingEachOtherIntersect) {
    using T = TypeParam;
    auto a = MakeTriangle<T>({ -5, -5, 0 }, { 5, -5, 0 }, { 0, 5, 0 });
    auto b = MakeTriangle<T>({ 0, 0, -5 }, { 0, 0, 5 }, { 0, 3, 0 });

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TYPED_TEST(HaveIntersection, ParallelPlanesDoNotIntersect) {
    using T = TypeParam;
    auto b = MakeTriangle<T>({ 0, 0, 5 }, { 4, 0, 5 }, { 0, 4, 5 });

    EXPECT_FALSE(kOriginRightTriangleLarge<T>.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(kOriginRightTriangleLarge<T>));
}

TYPED_TEST(HaveIntersection, ParallelPlanesDoNotIntersectSecond) {
    using T = TypeParam;
    auto a = MakeTriangle<T>({ 0, 0, 0 }, { 1, 0, 0 }, { 2, 0, 0 });
    auto b = MakeTriangle<T>({ 0, 1, 0 }, { 1, 1, 0 }, { 2, 1, 0 });

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TYPED_TEST(HaveIntersection, NonCoplanarTrianglesSeparatedByPlaneDoNotIntersect) {
    using T = TypeParam;
    auto a = MakeTriangle<T>({ 0, 0, -1 }, { 3, 0, 0 }, { 0, 3, -2 });
    auto b = MakeTriangle<T>({ 0, 0, 3 }, { 3, 0, 4 }, { 0, 3, 5 });

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TYPED_TEST(HaveIntersection, NonCoplanarTrianglesTouchingAtSinglePointIntersect) {
    using T = TypeParam;
    auto b = MakeTriangle<T>({ 0, 0, 0 }, { 0, 2, 2 }, { 0, -2, 2 });

    EXPECT_TRUE(kOriginRightTriangleSmall<T>.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall<T>));
}

TYPED_TEST(HaveIntersection, TwoDegenerateTrianglesOverlappingSegmentsIntersect) {
    using T = TypeParam;
    const T h = static_cast<T>(0.5);
    auto a = MakeTriangle<T>({ 0, 0, 0 }, { 1, 0, 0 }, { 2, 0, 0 });
    auto b = MakeTriangle<T>({ 1 + h, 0, 0 }, { 2 + h, 0, 0 }, { 3 + h, 0, 0 });

    EXPECT_TRUE(a.HaveIntersection(b));
}

TYPED_TEST(HaveIntersection, OverlapWithoutSharedVertices) {
    using T = TypeParam;
    auto a = MakeTriangle<T>({ 0, 0, 0 }, { 4, 0, 0 }, { 2, 3, 0 });
    auto b = MakeTriangle<T>({ 0, 2, 0 }, { 4, 2, 0 }, { 2, -1, 0 });

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TYPED_TEST(HaveIntersection, DetectsVertexLyingOnEdge) {
    using T = TypeParam;
    auto b = MakeTriangle<T>({ 1, 1, 0 }, { 3, 1, 0 }, { 1, 3, 0 });

    EXPECT_TRUE(kOriginRightTriangleSmall<T>.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(kOriginRightTriangleSmall<T>));
}

TYPED_TEST(HaveIntersection, SegmentCrossesThroughTriangle) {
    using T = TypeParam;
    auto b = MakeTriangle<T>({ 0, 0, 0 }, { 4, 0, 0 }, { 0, 4, 0 });
    auto a = MakeTriangle<T>({ -1, 2, 0 }, { 5, 2, 0 }, { 2, 2, 0 });

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TYPED_TEST(HaveIntersection, SeparatedSegmentsDoNotTouch) {
    using T = TypeParam;
    auto a = MakeTriangle<T>({ 0, 0, 0 }, { 1, 0, 0 }, { 2, 0, 0 });
    auto b = MakeTriangle<T>({ 5, 0, 0 }, { 6, 0, 0 }, { 7, 0, 0 });

    EXPECT_FALSE(a.HaveIntersection(b));
    EXPECT_FALSE(b.HaveIntersection(a));
}

TYPED_TEST(HaveIntersection, MatchesIdenticalSegments) {
    using T = TypeParam;
    auto a = MakeTriangle<T>({ 0, 0, 0 }, { 2, 0, 0 }, { 4, 0, 0 });

    EXPECT_TRUE(a.HaveIntersection(a));
}

TYPED_TEST(HaveIntersection, SegmentPiercesFace) {
    using T = TypeParam;
    auto b = MakeTriangle<T>({ -2, -2, 0 }, { 2, -2, 0 }, { 0, 2, 0 });
    auto a = MakeTriangle<T>({ 0, 0, -3 }, { 0, 0, 3 }, { 0, 0, 0 });

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TYPED_TEST(HaveIntersection, ApexTouchesFaceInterior) {
    using T = TypeParam;
    auto b = MakeTriangle<T>({ -3, -3, 0 }, { 3, -3, 0 }, { 0, 4, 0 });
    auto a = MakeTriangle<T>({ 0, 0, 0 }, { 1, 1, 3 }, { -1, 1, 3 });

    EXPECT_TRUE(a.HaveIntersection(b));
    EXPECT_TRUE(b.HaveIntersection(a));
}

TYPED_TEST(HaveIntersection, DetectsThinSliverOverlap) {
    using T = TypeParam;
    const T thin = static_cast<T>(2.01);
    auto big = MakeTriangle<T>({ 0, 0, 0 }, { 10, 0, 0 }, { 0, 10, 0 });
    auto sliver = MakeTriangle<T>({ -1, 2, 0 }, { 11, 2, 0 }, { 5, thin, 0 });

    EXPECT_TRUE(big.HaveIntersection(sliver));
    EXPECT_TRUE(sliver.HaveIntersection(big));
}

TYPED_TEST(HaveIntersectionMany, TrianglesSharingOneVertexAllIntersect) {
    using T = TypeParam;
    std::vector<Geometry::Triangle<T>> ts;
    for (int i = 1; i <= 300; i++) {
        const T f = static_cast<T>(i);
        ts.push_back(MakeTriangle<T>({ 0, 0, 0 }, { 1, f, 1 }, { f, 1, -1 }));
    }

    for (size_t i = 0; i < ts.size(); i++) {
        for (size_t j = i + 1; j < ts.size(); j++) {
            EXPECT_TRUE(ts[i].HaveIntersection(ts[j])) << i << " " << j;
            EXPECT_TRUE(ts[j].HaveIntersection(ts[i])) << j << " " << i;
        }
    }
}

TYPED_TEST(HaveIntersectionMany, TrianglesSharingOneEdgeAllIntersect) {
    using T = TypeParam;
    std::vector<Geometry::Triangle<T>> ts;
    for (int i = 1; i <= 300; i++) {
        const T f = static_cast<T>(i);
        ts.push_back(MakeTriangle<T>({ 0, 0, -1 }, { 0, 0, 1 }, { f, 1, 0 }));
    }

    for (size_t i = 0; i < ts.size(); i++) {
        for (size_t j = i + 1; j < ts.size(); j++) {
            EXPECT_TRUE(ts[i].HaveIntersection(ts[j])) << i << " " << j;
            EXPECT_TRUE(ts[j].HaveIntersection(ts[i])) << j << " " << i;
        }
    }
}

TYPED_TEST(HaveIntersectionMany, NestedCoplanarTrianglesAllIntersect) {
    using T = TypeParam;
    std::vector<Geometry::Triangle<T>> ts;
    for (int i = 1; i <= 300; i++) {
        const T f = static_cast<T>(i);
        ts.push_back(MakeTriangle<T>({ -f, -f, 0 }, { f, -f, 0 }, { 0, f, 0 }));
    }

    for (size_t i = 0; i < ts.size(); i++) {
        for (size_t j = i + 1; j < ts.size(); j++) {
            EXPECT_TRUE(ts[i].HaveIntersection(ts[j])) << i << " " << j;
            EXPECT_TRUE(ts[j].HaveIntersection(ts[i])) << j << " " << i;
        }
    }
}

TYPED_TEST(HaveIntersectionMany, ChainTouchingAtVerticesIntersectsOnlyNeighbours) {
    using T = TypeParam;
    std::vector<Geometry::Triangle<T>> ts;
    for (int i = 0; i < 300; i++) {
        const T x = static_cast<T>(2 * i);
        ts.push_back(MakeTriangle<T>({ x, 0, 0 }, { x + 2, 0, 0 }, { x, 2, 0 }));
    }

    for (size_t i = 0; i < ts.size(); i++) {
        for (size_t j = i + 1; j < ts.size(); j++) {
            const bool expected = (j - i == 1);
            EXPECT_EQ(ts[i].HaveIntersection(ts[j]), expected) << i << " " << j;
            EXPECT_EQ(ts[j].HaveIntersection(ts[i]), expected) << j << " " << i;
        }
    }
}

TYPED_TEST(HaveIntersectionMany, LongNeedlePiercesWholeStack) {
    using T = TypeParam;
    const int n = 300;
    std::vector<Geometry::Triangle<T>> stack;
    for (int i = 0; i < n; i++) {
        const T z = static_cast<T>(i);
        stack.push_back(MakeTriangle<T>({ 0, 0, z }, { 4, 0, z }, { 0, 4, z }));
    }
    const T half = static_cast<T>(0.5);
    const T quarter_more = static_cast<T>(0.75);
    const T top = static_cast<T>(n);
    auto needle = MakeTriangle<T>({ half, half, -1 }, { half, half, top }, { quarter_more, half, top });

    for (size_t i = 0; i < stack.size(); i++) {
        EXPECT_TRUE(needle.HaveIntersection(stack[i])) << i;
        EXPECT_TRUE(stack[i].HaveIntersection(needle)) << i;
        for (size_t j = i + 1; j < stack.size(); j++) {
            EXPECT_FALSE(stack[i].HaveIntersection(stack[j])) << i << " " << j;
        }
    }
}
