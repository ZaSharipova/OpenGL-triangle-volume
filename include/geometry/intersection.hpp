#ifndef GEOMETRY_INTERSECTION_HPP
#define GEOMETRY_INTERSECTION_HPP

#pragma once

#include "segment.hpp"
#include "vec3.hpp"

namespace geometry {

bool IsPointOnSegment(const geometry::Vec3& p, const Segment& s);
bool IsPointInTriangle(const geometry::Vec3& p, const geometry::Vec3& v0, const geometry::Vec3& v1, const geometry::Vec3& v2);
bool DoSegmentsIntersect(const Segment& s1, const Segment& s2);
bool DoesSegmentIntersectTriangle(const Segment& s, const geometry::Vec3& v0, const geometry::Vec3& v1, const geometry::Vec3& v2);

}  // namespace geometry

#endif // GEOMETRY_INTERSECTION_HPP
