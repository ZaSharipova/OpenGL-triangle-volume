#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <limits>
#include <utility>
#include <vector>

#include "intersection.hpp"
#include "segment.hpp"
#include "shape_type.hpp"
#include "vec3.hpp"

namespace Geometry {

template <typename T>
class Triangle;

namespace detail {

template <typename T>
bool HaveOverlap(const std::pair<T, T>& A, const std::pair<T, T>& B) {
    T A_1 = A.first, A_2 = A.second, B_1 = B.first, B_2 = B.second;

    return !(A_1 > B_2 || B_1 > A_2);
}

template <typename T>
std::array<Vec3<T>, 17> FindAxes(const Triangle<T>& tr1, const Triangle<T>& tr2) {
    std::array<Vec3<T>, 17> res_array { };
    size_t res_array_index = 0;
    Vec3<T> normalA = tr1.FindNormal();
    Vec3<T> normalB = tr2.FindNormal();
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
} // namespace detail

template <typename T>
class Triangle {
public:
    Triangle(Vec3<T> v1, Vec3<T> v2, Vec3<T> v3)
        : vertices { v1, v2, v3 } { }

    Vec3<T> GetEdge(int index) const {
        return vertices[(index + 1) % 3] - vertices[index];
    }

    Vec3<T> FindNormal() const {
        return GetEdge(0).FindCross(GetEdge(1));
    }

    bool HaveIntersection(const Triangle& other) const {
        const ShapeType lhs = FindShapeType();
        const ShapeType rhs = other.FindShapeType();

        if (lhs != ShapeType::kTriangle || rhs != ShapeType::kTriangle) {
            return HaveDegenerateIntersection(other, rhs);
        }
        return HaveIntersectionUsingSAT(other);
    }

    const Vec3<T>& GetVertex(size_t index) const {
        assert(index <= 2);
        return vertices[index];
    }

    ShapeType FindShapeType() const {
        Vec3<T> normal = FindNormal();
        if (!IsZero(normal.FindDot(normal))) {
            return ShapeType::kTriangle;
        }

        if (GetVertex(0) == GetVertex(1) && GetVertex(0) == GetVertex(2)) {
            return ShapeType::kPoint;
        }

        return ShapeType::kSegment;
    }

private:
    std::pair<T, T> FindMinMaxCoordsOnAxis(const Vec3<T>& axis) const {
        T min_param = std::numeric_limits<T>::max();
        T max_param = std::numeric_limits<T>::lowest();
        for (auto value : vertices) {
            T value_coord = value.FindDot(axis);
            min_param = std::min(min_param, value_coord);
            max_param = std::max(max_param, value_coord);
        }

        return { min_param, max_param };
    }

    bool HaveIntersectionUsingSAT(const Triangle& other) const {
        std::array<Vec3<T>, 17> axes = detail::FindAxes(*this, other);
        for (const auto& axis : axes) {
            if (axis.IsDegenerate()) {
                continue;
            }

            std::pair<T, T> intervalA = FindMinMaxCoordsOnAxis(axis);
            std::pair<T, T> intervalB = other.FindMinMaxCoordsOnAxis(axis);
            if (!detail::HaveOverlap(intervalA, intervalB)) {
                return false;
            }
        }

        return true;
    }

    bool HaveDegenerateIntersection(const Triangle& other, ShapeType rhsShapeType) const {
        const ShapeType lhsShapeType = FindShapeType();

        if (lhsShapeType > rhsShapeType) {
            return other.HaveDegenerateIntersection(*this, lhsShapeType);
        }

        switch (lhsShapeType) {
        case ShapeType::kPoint: {
            const Vec3<T> point = GetVertex(0);
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

    Segment<T> GetLongestEdge() const {
        const Vec3<T> a = GetVertex(0);
        const Vec3<T> b = GetVertex(1);
        const Vec3<T> c = GetVertex(2);

        const std::array<Segment<T>, 3> edges = { { { a, b }, { b, c }, { c, a } } };

        auto LengthSq = [](const Segment<T>& seg) {
            const Vec3<T> d = seg.b - seg.a;
            return Dot(d, d);
        };

        return *std::max_element(edges.begin(), edges.end(),
            [&LengthSq](const Segment<T>& lhs, const Segment<T>& rhs) {
                return LengthSq(lhs) < LengthSq(rhs);
            });
    }

    std::array<Vec3<T>, 3> vertices;
};

} // namespace Geometry

#endif // TRIANGLE_HPP_
