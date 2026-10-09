#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "vec3.hpp"

struct Camera {
    Geometry::Vec3 pos {0.0f, 0.0f, 3.0f};
    Geometry::Vec3 front {0.0f, 0.0f, -1.0f};
    Geometry::Vec3 up {0.0f, 1.0f, 0.0f};

    float yaw = -90.0f;
    float pitch = 0.0f;
};

#endif // CAMERA_HPP_
