#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include <cmath>
#include <limits>
#include <array>
#include <utility>
#include <algorithm>
#include <vector>

#include "shape_type.hpp"
#include "segment.hpp"
#include "vec3.hpp"

namespace Geometry {

class Triangle {
public:
    Triangle(Vec3 v1, Vec3 v2, Vec3 v3) : vertices{v1, v2, v3} {}

    Vec3 GetEdge(int index) const;
    Vec3 FindNormal() const;
    bool HaveIntersection(const Triangle& other) const;
    const Vec3& GetVertex(size_t index) const;
    ShapeType FindShapeType() const;

private:
    std::pair<float, float> FindMinMaxCoordsOnAxis(const Vec3& axis) const;
    bool HaveIntersectionUsingSAT(const Triangle& other) const;
    bool HaveDegenerateIntersection(const Triangle& other, ShapeType lhsShapeType) const;
    Segment GetLongestEdge() const;

    Vec3 vertices[3];
};

std::vector<bool> FindIntersectingFlags(const std::vector<Triangle>& triangles);

} // namespace Geometry

#endif // TRIANGLE_HPP_
