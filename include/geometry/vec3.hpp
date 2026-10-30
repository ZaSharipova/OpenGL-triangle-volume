#ifndef GEOMETRY_VEC_HPP_
#define GEOMETRY_VEC_HPP_

namespace Geometry {

class Vec3 {
public:
    friend bool operator==(const Vec3& lhs, const Vec3& rhs);

    Vec3(float x, float y, float z) : x_(x), y_(y), z_(z) {}
    Vec3() = default;

    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(const float factor) const;
    Vec3 operator/(const float divisor) const;
    Vec3 FindCross(const Vec3& other) const;
    float FindDot(const Vec3& other) const;
    Vec3 Normalize();
    float FindLength() const;
    bool IsDegenerate() const;
    float x_ = 0.0, y_ = 0.0, z_ = 0.0;
};

inline float Dot(const Vec3& a, const Vec3& b) {
    return a.FindDot(b);
}

inline Vec3 Cross(const Vec3& a, const Vec3& b) {
    return a.FindCross(b);
}

} // namespace Geometry

#endif // GEOMETRY_VEC_HPP_
