#ifndef SUBSIDIARY_HPP_
#define SUBSIDIARY_HPP_

#include "triangle.hpp"

#include <optional>

std::optional<std::vector<Triangle>> ReadTriangles(std::istream& in);
std::vector<float> FlattenVertices(const std::vector<Triangle>& triangles, const std::vector<bool>& intersecting);

struct Matrix4x4 {
    float matrix[4][4];
};

Matrix4x4 Identity();
Matrix4x4 Perspective(float fov, float aspect, float near, float far);
Matrix4x4 ViewMatrix(const Vec3& eye, const Vec3& target, const Vec3& up);

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
