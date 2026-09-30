#ifndef GEOMETRY_SUBSIDIARY_HPP_
#define GEOMETRY_SUBSIDIARY_HPP_

#include "vec3.hpp"

namespace geometry {

inline constexpr float kEps = 1e-6f;

bool AreEqual(const float number_1, const float number_2);
bool IsZero(float vec);
bool IsZero(const geometry::Vec3& vec);

} // namespace geometry

#endif // GEOMETRY_SUBSIDIARY_HPP_
