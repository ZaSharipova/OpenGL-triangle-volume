#ifndef SUBSIDIARY_HPP_
#define SUBSIDIARY_HPP_

#include "triangle.hpp"
#include "vec3.hpp"
#include "config.hpp"

#include <vector>

namespace Interface {

namespace Subsidiary {

    constexpr size_t kNumberOfVerticesInCube = 8;
    constexpr size_t kNumberOfEdgesInCube = 12;

    std::vector<float> FlattenVertices(const std::vector<Geometry::Trianglef>& triangles, const std::vector<bool>& intersecting);

    struct Matrix4x4 {
        float matrix[4][4];
    };

    Matrix4x4 Identity();
    Matrix4x4 Perspective(float fov, float aspect, float near, float far);
    Matrix4x4 ViewMatrix(const Geometry::Vec3f& eye, const Geometry::Vec3f& target, const Geometry::Vec3f& up);
    std::array<Geometry::Vec3f, kNumberOfVerticesInCube> FindCubeCorners(const std::pair<Geometry::Vec3f, Geometry::Vec3f>& boundingBox);
    std::vector<float> FindCubeEdgePoints(const std::array<Geometry::Vec3f, kNumberOfVerticesInCube>& corners);

} // namespace Subsidiary

} // namespace Interface

#endif // SUBSIDIARY_HPP_
