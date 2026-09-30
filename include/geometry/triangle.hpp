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

namespace geometry {

class Triangle {
public:
    Triangle(geometry::Vec3 v1, geometry::Vec3 v2, geometry::Vec3 v3) : vertices{v1, v2, v3} {}

    geometry::Vec3 GetEdge(int index) const;
    geometry::Vec3 FindNormal() const;
    bool HaveIntersection(const Triangle& other) const;
    const geometry::Vec3& GetVertex(size_t index) const;
    geometry::ShapeType FindShapeType() const;

private:
    std::pair<float, float> FindMinMaxCoordsOnAxis(const geometry::Vec3& axis) const;
    bool HaveIntersectionUsingSAT(const Triangle& other) const;
    bool HaveDegenerateIntersection(const Triangle& other, geometry::ShapeType lhsShapeType) const;
    Segment GetLongestEdge() const;

    geometry::Vec3 vertices[3];
};

std::vector<bool> FindIntersectingFlags(const std::vector<Triangle>& triangles);

} // namespace geometry

#endif // TRIANGLE_HPP_
