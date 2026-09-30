#include "triangle.hpp"
#include "intersection.hpp"
#include "vec3.hpp"
#include "geometry_subsidiary.hpp"

#include <utility>

namespace geometry {

std::array<Vec3, 17> FindAxes(const Triangle& tr1, const Triangle& tr2);
bool HaveOverlap(const std::pair<float, float>& A, const std::pair<float, float>& B);

//------------------------------------------
Vec3 Triangle::GetEdge(int index) const {
    return vertices[(index + 1) % 3] - vertices[index];
}

Vec3 Triangle::FindNormal() const {
    return GetEdge(0).FindCross(GetEdge(1));
}

bool Triangle::HaveIntersectionUsingSAT(const Triangle& other) const {
    std::array<Vec3, 17> axes = FindAxes(*this, other);
    for (const auto& axis : axes) {
        if (axis.IsDegenerate()) {
            continue;
        }

        std::pair<float, float> intervalA = FindMinMaxCoordsOnAxis(axis);
        std::pair<float, float> intervalB = other.FindMinMaxCoordsOnAxis(axis);
        if (!HaveOverlap(intervalA, intervalB)) {
            return false;
        }
    }

    return true;
}

std::pair<float, float> Triangle::FindMinMaxCoordsOnAxis(const Vec3& axis) const {
    float min_param = std::numeric_limits<float>::max();
    float max_param = std::numeric_limits<float>::lowest();
    for (auto value : vertices) {
        float value_coord = value.FindDot(axis);
        min_param = std::min(min_param, value_coord);
        max_param = std::max(max_param, value_coord);
    }

    return {min_param, max_param};
}

const Vec3& Triangle::GetVertex(size_t index) const {
    return vertices[index];
}

ShapeType Triangle::FindShapeType() const {
    Vec3 normal = FindNormal();
    if (!AreEqual(normal.FindDot(normal), 0)) {
        return ShapeType::kTriangle;
    }

    if (GetVertex(0) == GetVertex(1) && GetVertex(0) == GetVertex(2)) {
        return ShapeType::kPoint;
    }

    return ShapeType::kSegment;
}

//------------------------------------------
bool HaveOverlap(const std::pair<float, float>& A, const std::pair<float, float>& B) {
    float A_1 = A.first, A_2 = A.second, B_1 = B.first, B_2 = B.second;

    return !(A_1 > B_2 || B_1 > A_2);
}

std::array<Vec3, 17> FindAxes(const Triangle& tr1, const Triangle& tr2) {
    std::array<Vec3, 17> res_array {};
    size_t res_array_index = 0;
    Vec3 normalA = tr1.FindNormal();
    Vec3 normalB = tr2.FindNormal();
    res_array[res_array_index++] = normalA;
    res_array[res_array_index++] = normalB;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            res_array[res_array_index++] = tr1.GetEdge(i).FindCross(tr2.GetEdge(j));
        }
    }

    for (size_t i = 0; i < 3; i++) {
        res_array[res_array_index++] = tr1.GetEdge(i).FindCross(normalA);
    }

    for (size_t i = 0; i < 3; i++) {
        res_array[res_array_index++] = tr2.GetEdge(i).FindCross(normalB);
    }

    return res_array;
}

bool Triangle::HaveIntersection(const Triangle& other) const {
    const ShapeType lhs = FindShapeType();
    const ShapeType rhs = other.FindShapeType();

    if (lhs != ShapeType::kTriangle || rhs != ShapeType::kTriangle) {
        return HaveDegenerateIntersection(other, rhs);
    }
    return HaveIntersectionUsingSAT(other);
}

Segment Triangle::GetLongestEdge() const {
    const Vec3 vec[3] = {GetVertex(0), GetVertex(1), GetVertex(2)};
    Segment best{vec[0], vec[1]};
    float bestLen = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            const Vec3 dist = vec[j] - vec[i];
            const float len = Dot(dist, dist);

            if (len > bestLen) {
                bestLen = len;
                best = {vec[i], vec[j]};
            }
        }
    }

    return best;
}

bool Triangle::HaveDegenerateIntersection(const Triangle& other, ShapeType rhsShapeType) const {
    const ShapeType lhsShapeType = FindShapeType();

    if (lhsShapeType > rhsShapeType) {
        return other.HaveDegenerateIntersection(*this, lhsShapeType);
    }

    switch (lhsShapeType) {
        case ShapeType::kPoint: {
            const Vec3 point = GetVertex(0);
            switch (rhsShapeType) {
                case ShapeType::kPoint:
                    return IsZero(point - other.GetVertex(0));
                case ShapeType::kSegment:
                    return IsPointOnSegment(point, other.GetLongestEdge());
                case ShapeType::kTriangle:
                    return IsPointInTriangle(point, other.GetVertex(0), other.GetVertex(1),
                                           other.GetVertex(2));
            }

            break;
        }

        case ShapeType::kSegment: {
            const Segment segment = GetLongestEdge();
            switch (rhsShapeType) {
                case ShapeType::kSegment:
                    return DoSegmentsIntersect(segment, other.GetLongestEdge());
                case ShapeType::kTriangle:
                    return DoesSegmentIntersectTriangle(segment, other.GetVertex(0), other.GetVertex(1),
                                                     other.GetVertex(2));
                default:
                    break;
            }

            break;
        }

        default:
            break;
    }
    return false;
}

} // namespace geometry
