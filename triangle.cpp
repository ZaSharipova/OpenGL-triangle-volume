#include "triangle.hpp"

#include <cmath>
#include <limits>
#include <array>
#include <utility>
#include <algorithm>

class Vec3;
class Triangle;

std::array<Vec3, 11> FindAxes(const Triangle& tr1, const Triangle& tr2);
bool HaveOverlap(const std::pair<float, float>& A, const std::pair<float, float>& B);
Vec3 Normalize(const Vec3& vec);

bool AreEqual(const float number_1, const float number_2) {
    return std::abs(number_1 - number_2) < 1e-9;
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}

Vec3 Vec3::operator/(float divisor) const {
    return Vec3(x_ / divisor, y_ / divisor, z_ / divisor);
}

Vec3 Vec3::FindCross(const Vec3& other) const {
    return Vec3(y_ * other.z_ - z_ * other.y_,
                z_ * other.x_ - x_ * other.z_,
                x_ * other.y_ - y_ * other.x_);
}

float Vec3::FindDot(const Vec3& other) const {
    return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

float Vec3::FindLength() const {
    return std::sqrt(FindDot(*this));
}

bool Vec3::IsDegenerate() const {
    return AreEqual(x_, 0.0f) && AreEqual(y_, 0.0f) && AreEqual(z_, 0.0f);
}

//------------------------------------------
Vec3 Triangle::FindEdge(int index) const {
    return vertices[(index + 1) % 3] - vertices[index];
}

Vec3 Triangle::FindNormal() const {
    return FindEdge(0).FindCross(FindEdge(1));
}

bool Triangle::HaveIntersection(const Triangle& other) const {
    std::array<Vec3, 11> axes = FindAxes(*this, other);
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

//------------------------------------------
bool HaveOverlap(const std::pair<float, float>& A, const std::pair<float, float>& B) {
    float A_1 = A.first, A_2 = A.second, B_1 = B.first, B_2 = B.second;

    return !(A_1 > B_2 || B_1 > A_2);
}

std::array<Vec3, 11> FindAxes(const Triangle& tr1, const Triangle& tr2) {
    std::array<Vec3, 11> res_array {};
    size_t res_array_index = 0;
    res_array[res_array_index++] = tr1.FindNormal();
    res_array[res_array_index++] = tr2.FindNormal();

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            res_array[res_array_index++] = tr1.FindEdge(i).FindCross(tr2.FindEdge(j));
        }
    }

    return res_array;
}

Vec3 Normalize(const Vec3& vec) {
    float length = vec.FindLength();
    if (AreEqual(length, 0.0f)) {
        return {0.0, 0.0, 0.0}; // TODO
    }

    return vec / length;
}
