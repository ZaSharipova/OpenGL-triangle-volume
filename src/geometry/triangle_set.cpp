#include "triangle_set.hpp"

#include <vector>

namespace geometry {

std::vector<bool> FindIntersectingFlags(const std::vector<Triangle>& triangles) {
    std::vector<bool> intersecting(triangles.size(), false);

    for (size_t i = 0; i < triangles.size(); i++) {
        for (size_t j = i + 1; j < triangles.size(); j++) {
            if (triangles[i].HaveIntersection(triangles[j])) {
                intersecting[i] = intersecting[j] = true;
            }
        }
    }

    return intersecting;
}

} // namespace geometry
