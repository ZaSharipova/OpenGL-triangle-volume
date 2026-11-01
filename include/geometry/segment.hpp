#ifndef GEOMETRY_SEGMENT_HPP_
#define GEOMETRY_SEGMENT_HPP_

#include "vec3.hpp"

namespace Geometry {

template <typename T>
struct Segment {
    Vec3<T> a;
    Vec3<T> b;
};

} // namespace Geometry

#endif // GEOMETRY_SEGMENT_HPP_
