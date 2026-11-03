#ifndef GEOMETRY_INTERSECTION_HPP
#define GEOMETRY_INTERSECTION_HPP

#include "segment.hpp"
#include "vec3.hpp"

#include <iostream>
#include <algorithm>

namespace Geometry {

template <typename T>
bool IsPointOnSegment(const Vec3<T>& point, const Segment<T>& segment) {
    const Vec3<T> dir = segment.b - segment.a;
    const Vec3<T> ap = point - segment.a;
    if (!IsZero(Cross(dir, ap))) {
        return false;
    }

    const T t = Dot(ap, dir) / Dot(dir, dir);
    return t >= -kEps<T> && t <= 1 + kEps<T>;
}

template <typename T>
bool IsPointInTriangle(const Vec3<T>& point, const Vec3<T>& v0, const Vec3<T>& v1, const Vec3<T>& v2) {
    const Vec3<T> normal = Cross(v1 - v0, v2 - v0);
    const T normalLength = std::sqrt(Dot(normal, normal));

    const T signedDistanceToPlane = Dot(normal, point - v0);
    if (std::abs(signedDistanceToPlane) > kEps<T> * normalLength) {
        return false;
    }

    const T sideOfEdge01 = Dot(Cross(v1 - v0, point - v0), normal);
    const T sideOfEdge12 = Dot(Cross(v2 - v1, point - v1), normal);
    const T sideOfEdge20 = Dot(Cross(v0 - v2, point - v2), normal);

    const T threshold = -kEps<T> * normalLength;
    return sideOfEdge01 >= threshold && sideOfEdge12 >= threshold && sideOfEdge20 >= threshold;
}

template <typename T>
bool DoSegmentsIntersect(const Segment<T>& first, const Segment<T>& second) {
    const Vec3<T> firstDirection = first.b - first.a;
    const Vec3<T> secondDirection = second.b - second.a;
    const Vec3<T> startOffset = second.a - first.a;

    const Vec3<T> directionsCross = Cross(firstDirection, secondDirection);
    const T directionsCrossLengthSquared = Dot(directionsCross, directionsCross);

    if (directionsCrossLengthSquared > kEps<T> * kEps<T>) {
        if (!IsZero(Dot(startOffset, directionsCross))) {
            return false;
        }

        const T firstParam = Dot(Cross(startOffset, secondDirection), directionsCross) / directionsCrossLengthSquared;
        const T secondParam = Dot(Cross(startOffset, firstDirection), directionsCross) / directionsCrossLengthSquared;

        return firstParam >= -kEps<T> && firstParam <= 1 + kEps<T> && secondParam >= -kEps<T> && secondParam <= 1 + kEps<T>;
    }

    if (!IsZero(Cross(startOffset, firstDirection))) {
        return false;
    }

    const T firstLengthSquared = Dot(firstDirection, firstDirection);
    const T secondStartParam = Dot(startOffset, firstDirection) / firstLengthSquared;
    const T secondEndParam = Dot(second.b - first.a, firstDirection) / firstLengthSquared;

    const T overlapStart = std::max(std::min(secondStartParam, secondEndParam), T());
    const T overlapEnd = std::min(std::max(secondStartParam, secondEndParam), static_cast<T>(T() + 1));

    return overlapStart <= overlapEnd + kEps<T>;
}

template <typename T>
bool DoesSegmentIntersectTriangle(const Segment<T>& segment, const Vec3<T>& v0, const Vec3<T>& v1, const Vec3<T>& v2) {
    const Vec3<T> normal = Cross(v1 - v0, v2 - v0);

    const T startDistance = Dot(normal, segment.a - v0);
    const T endDistance = Dot(normal, segment.b - v0);

    if (IsZero(startDistance) && IsZero(endDistance)) {
        if (IsPointInTriangle(segment.a, v0, v1, v2) || IsPointInTriangle(segment.b, v0, v1, v2)) {
            return true;
        }

        return DoSegmentsIntersect(segment, { v0, v1 })
            || DoSegmentsIntersect(segment, { v1, v2 })
            || DoSegmentsIntersect(segment, { v2, v0 });
    }

    if (startDistance * endDistance > 0) {
        return false;
    }

    const T crossingRatio = startDistance / (startDistance - endDistance);
    const Vec3<T> planeIntersection = segment.a + (segment.b - segment.a) * crossingRatio;

    return IsPointInTriangle(planeIntersection, v0, v1, v2);
}

} // namespace Geometry

#endif // GEOMETRY_INTERSECTION_HPP
