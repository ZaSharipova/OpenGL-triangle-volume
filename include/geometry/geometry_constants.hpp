#ifndef GEOMETRY_CONSTANTS_HPP_
#define GEOMETRY_CONSTANTS_HPP_

#include <cmath>
#include <type_traits>
#include <limits>

namespace Geometry {

template <typename T>
constexpr T kEps = std::numeric_limits<T>::epsilon();

template <typename T>
    requires std::is_floating_point_v<T>
bool AreEqual(const T number1, const T number2) {
    return std::abs(number1 - number2) < kEps<T>;
}

template <typename T>
    requires std::is_floating_point_v<T>
bool IsZero(T vec) {
    return AreEqual(vec, T());
}

} // namespace Geometry

#endif // GEOMETRY_CONSTANTS_HPP_
