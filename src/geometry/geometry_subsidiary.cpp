#include "geometry_subsidiary.hpp"

#include <cmath>

namespace geometry {
bool AreEqual(const float number1, const float number2) {
    return std::abs(number1 - number2) < kEps;
}

bool IsZero(float vec) {
    return AreEqual(vec, 0.0);
}

bool IsZero(const Vec3& vec) {
    return Dot(vec, vec) <= kEps * kEps;
}
} // namespace geometry
