#include "intersection.hpp"
#include "segment.hpp"
#include "vec3.hpp"
#include "geometry_subsidiary.hpp"

#include <iostream>

namespace Geometry {

bool IsPointOnSegment(const Vec3& point, const Segment& segment) {
    const Vec3 dir = segment.b - segment.a;
    const Vec3 ap = point - segment.a;
    if (!IsZero(Cross(dir, ap))) {
        return false;
    }

    const float t = Dot(ap, dir) / Dot(dir, dir);
    return t >= -kEps && t <= 1 + kEps;
}

bool IsPointInTriangle(const Vec3& point, const Vec3& v0, const Vec3& v1, const Vec3& v2) {
    const Vec3 normal = Cross(v1 - v0, v2 - v0);
    const float normalLength = std::sqrt(Dot(normal, normal));

    const float signedDistanceToPlane = Dot(normal, point - v0);
    if (std::abs(signedDistanceToPlane) > kEps * normalLength) {
        return false;
    }

    const float sideOfEdge01 = Dot(Cross(v1 - v0, point - v0), normal);
    const float sideOfEdge12 = Dot(Cross(v2 - v1, point - v1), normal);
    const float sideOfEdge20 = Dot(Cross(v0 - v2, point - v2), normal);

    const float threshold = -kEps * normalLength;
    return sideOfEdge01 >= threshold && sideOfEdge12 >= threshold && sideOfEdge20 >= threshold;
}

bool DoSegmentsIntersect(const Segment& first, const Segment& second) {
    const Vec3 firstDirection = first.b - first.a;
    const Vec3 secondDirection = second.b - second.a;
    const Vec3 startOffset = second.a - first.a;

    const Vec3 directionsCross = Cross(firstDirection, secondDirection);
    const float directionsCrossLengthSquared = Dot(directionsCross, directionsCross);

    if (directionsCrossLengthSquared > kEps * kEps) {
        if (!IsZero(Dot(startOffset, directionsCross))) {
            return false;
        }

        const float firstParam =
            Dot(Cross(startOffset, secondDirection), directionsCross) / directionsCrossLengthSquared;
        const float secondParam =
            Dot(Cross(startOffset, firstDirection), directionsCross) / directionsCrossLengthSquared;

        return firstParam >= -kEps && firstParam <= 1 + kEps &&
               secondParam >= -kEps && secondParam <= 1 + kEps;
    }

    if (!IsZero(Cross(startOffset, firstDirection))) {
        return false;
    }

    const float firstLengthSquared = Dot(firstDirection, firstDirection);
    const float secondStartParam = Dot(startOffset, firstDirection) / firstLengthSquared;
    const float secondEndParam = Dot(second.b - first.a, firstDirection) / firstLengthSquared;

    const float overlapStart = std::max(std::min(secondStartParam, secondEndParam), 0.0f);
    const float overlapEnd = std::min(std::max(secondStartParam, secondEndParam), 1.0f);

    return overlapStart <= overlapEnd + kEps;
}

bool DoesSegmentIntersectTriangle(const Segment& segment, const Vec3& v0, const Vec3& v1, const Vec3& v2) {
    const Vec3 normal = Cross(v1 - v0, v2 - v0);

    const float startDistance = Dot(normal, segment.a - v0);
    const float endDistance = Dot(normal, segment.b - v0);

    if (IsZero(startDistance) && IsZero(endDistance)) {
        if (IsPointInTriangle(segment.a, v0, v1, v2) || IsPointInTriangle(segment.b, v0, v1, v2)) {
            return true;
        }

        return DoSegmentsIntersect(segment, {v0, v1}) ||
               DoSegmentsIntersect(segment, {v1, v2}) ||
               DoSegmentsIntersect(segment, {v2, v0});
    }

    if (startDistance * endDistance > 0) {
        return false;
    }

    const float crossingRatio = startDistance / (startDistance - endDistance);
    const Vec3 planeIntersection = segment.a + (segment.b - segment.a) * crossingRatio;

    return IsPointInTriangle(planeIntersection, v0, v1, v2);
}

} // namespace Geometry
