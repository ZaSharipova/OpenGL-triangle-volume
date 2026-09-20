#ifndef SUBSIDIARY_HPP_
#define SUBSIDIARY_HPP_

#include "triangle.hpp"

struct Matrix4x4 {
    // Matrix4x4& operator+(const Matrix4x4& other) {
    //    return {};
    // }
    float matrix[4][4];
};

struct Camera {
    Vec3 pos {0.0f, 0.0f, 3.0f};
    Vec3 front {0.0f, 0.0f, -1.0f};
    Vec3 up {0.0f, 1.0f, 0.0f};

    float yaw = -90.0f;
    float pitch = 0.0f;

    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;

    float mouseSensitivity = 0.1f;
};

#endif // SUBSIDIARY_HPP_
