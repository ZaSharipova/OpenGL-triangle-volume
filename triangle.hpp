#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include <cmath>
#include <limits>
#include <array>
#include <utility>
#include <algorithm>

class Vec3 {
public:
    Vec3(float x, float y, float z) : x_(x), y_(y), z_(z) {}
    Vec3() = default;

    Vec3 operator-(const Vec3& other) const;
    Vec3 operator/(float divisor) const;
    Vec3 FindCross(const Vec3& other) const;
    float FindDot(const Vec3& other) const;
    float FindLength() const;
    bool IsDegenerate() const;

private:
    float x_, y_, z_;
};

class Triangle {
public:
    Triangle(Vec3 v1, Vec3 v2, Vec3 v3) : vertices{v1, v2, v3} {}

    Vec3 FindEdge(int index) const;
    Vec3 FindNormal() const;
    bool HaveIntersection(const Triangle& other) const;

private:
    std::pair<float, float> FindMinMaxCoordsOnAxis(const Vec3& axis) const;

    Vec3 vertices[3];
};

#endif // TRIANGLE_HPP_
