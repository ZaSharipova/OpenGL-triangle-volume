#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include <iostream>
// #include <cmath>

class Triangle {
public:
    Triangle(float x_coord, float y_coord, float z_coord) : x_coord(x_coord), y_coord(y_coord), z_coord(z_coord) {}

    float FindDist(Triangle& other) {
        return std::sqrt(std::pow(x_coord - other.x_coord, 2)
                       + std::pow(y_coord - other.y_coord, 2)
                       + std::pow(z_coord - other.z_coord, 2));
    }


private:
    float x_coord = 0, y_coord = 0, z_coord = 0;
};

#endif // TRIANGLE_HPP_
