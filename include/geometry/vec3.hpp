#ifndef GEOMETRY_VEC_HPP_
#define GEOMETRY_VEC_HPP_

#include <type_traits>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "geometry_constants.hpp"

namespace Geometry {

template <typename T>
    requires std::is_floating_point_v<T>
class Vec3 {
public:
    friend bool operator==(const Vec3& lhs, const Vec3& rhs) = default;

    Vec3(T x, T y, T z)
        : x_(x)
        , y_(y)
        , z_(z) { }
    Vec3() = default;

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
    }

    Vec3 operator-(const Vec3& other) const {
        return Vec3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
    }

    Vec3 operator*(const T factor) const {
        return Vec3(x_ * factor, y_ * factor, z_ * factor);
    }

    Vec3 operator/(const T divisor) const {
        if (IsZero(divisor)) {
            std::cerr << "Zero divizor\n";
            throw std::invalid_argument("Vec3 division by zero");
        }

        return Vec3(x_ / divisor, y_ / divisor, z_ / divisor);
    }

    Vec3 FindCross(const Vec3& other) const {
        return Vec3(y_ * other.z_ - z_ * other.y_,
            z_ * other.x_ - x_ * other.z_,
            x_ * other.y_ - y_ * other.x_);
    }

    T FindDot(const Vec3& other) const {
        return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
    }


    T FindLength() const {
        return std::sqrt(FindDot(*this));
    }

    bool IsDegenerate() const {
        return IsZero(x_) && IsZero(y_) && IsZero(z_);
    }

    Vec3 Normalize() {
        T length = FindLength();
        if (IsZero(length)) {
            return { 0.0, 0.0, 0.0 };
        }

        return *this / length;
    }

    T x_ = 0.0, y_ = 0.0, z_ = 0.0;
};

template <typename T>
inline T Dot(const Vec3<T>& a, const Vec3<T>& b) {
    return a.FindDot(b);
}

template <typename T>
inline Vec3<T> Cross(const Vec3<T>& a, const Vec3<T>& b) {
    return a.FindCross(b);
}

template <typename T>
bool IsZero(const Vec3<T>& vec) {
    return Dot(vec, vec) <= kEps<T> * kEps<T>;
}

} // namespace Geometry

#endif // GEOMETRY_VEC_HPP_
