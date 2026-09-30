#include "vec3.hpp"
#include "geometry_subsidiary.hpp"

#include <cmath>

namespace geometry {

Vec3 Vec3::operator+(const Vec3& other) const {
    return Vec3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}

Vec3 Vec3::operator*(const float factor) const {
    return Vec3(x_ * factor, y_ * factor, z_ * factor);
}

Vec3 Vec3::operator/(const float divisor) const {
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

Vec3 Vec3::Normalize() {
    float length = FindLength();
    if (AreEqual(length, 0.0f)) {
        return {0.0, 0.0, 0.0}; // TODO
    }

    return *this / length;
}

float Vec3::GetX() const {
    return x_;
}

float Vec3::GetY() const {
    return y_;
}

float Vec3::GetZ() const {
    return z_;
}

void Vec3::SetX(const float& x) {
    x_ = x;
}

void Vec3::SetY(const float& y) {
    y_ = y;
}

void Vec3::SetZ(const float& z) {
    z_= z;
}

bool operator==(const Vec3& lhs, const Vec3& rhs) {
    return AreEqual(lhs.GetX(), rhs.GetX()) && AreEqual(lhs.GetY(), rhs.GetY()) && AreEqual(lhs.GetZ(), rhs.GetZ());
}

} // namespace geometry
