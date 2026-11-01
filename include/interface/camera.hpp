#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "vec3.hpp"
#include "config.hpp"

namespace Interface {

struct Camera {
    Geometry::Vec3f pos { 0.0f, 0.0f, 3.0f };
    Geometry::Vec3f front { 0.0f, 0.0f, -1.0f };
    Geometry::Vec3f up { 0.0f, 1.0f, 0.0f };

    float yaw = -90.0f;
    float pitch = 0.0f;
};

} // namespace Interface

#endif // CAMERA_HPP_
