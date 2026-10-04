#include "triangle_set.hpp"
#include "geometry_subsidiary.hpp"
#include "subsidiary.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace geometry {

namespace {

using CellMap = std::unordered_map<uint64_t, std::vector<size_t>>;

std::array<float, 3> ToArray(const Vec3& v) {
    return {v.GetX(), v.GetY(), v.GetZ()};
}

struct Grid {
    std::array<float, 3> origin;
    std::array<float, 3> cell_width;
    int resolution;

    int CellIndex(float value, size_t axis) const {
        if (AreEqual(cell_width[axis], 0.0f)) {
            return 0;
        }

        const int index = static_cast<int>((value - origin[axis]) / cell_width[axis]);
        return std::clamp(index, 0, resolution - 1);
    }
};

uint64_t MakeKey(int cx, int cy, int cz) {
    return (static_cast<uint64_t>(cx) << 42)
           | (static_cast<uint64_t>(cy) << 21)
           | static_cast<uint64_t>(cz);
}

Grid BuildGrid(const std::vector<Triangle>& triangles) {
    const int resolution = std::cbrt(static_cast<double>(triangles.size() + 1));
    const auto [scene_min, scene_max] = FindBoundingBox(triangles);

    Grid grid;
    grid.resolution = resolution;
    grid.origin = ToArray(scene_min);
    const std::array<float, 3> max_arr = ToArray(scene_max);
    for (size_t axis = 0; axis < 3; axis++) {
        grid.cell_width[axis] = (max_arr[axis] - grid.origin[axis]) / resolution;
    }
    return grid;
}

CellMap BuildMap(const std::vector<Triangle>& triangles, const Grid& grid) {
    CellMap cells;

    for (size_t i = 0; i < triangles.size(); i++) {
        const auto [t_min, t_max] = FindBoundingBox(triangles[i]);
        const std::array<float, 3> lo = ToArray(t_min);
        const std::array<float, 3> hi = ToArray(t_max);

        std::array<int, 3> low_idx;
        std::array<int, 3> high_idx;
        for (size_t axis = 0; axis < 3; axis++) {
            low_idx[axis] = grid.CellIndex(lo[axis], axis);
            high_idx[axis] = grid.CellIndex(hi[axis], axis);
        }

        for (int cx = low_idx[0]; cx <= high_idx[0]; cx++) {
            for (int cy = low_idx[1]; cy <= high_idx[1]; cy++) {
                for (int cz = low_idx[2]; cz <= high_idx[2]; cz++) {
                    cells[MakeKey(cx, cy, cz)].push_back(i);
                }
            }
        }
    }

    return cells;
}

void CheckCell(const std::vector<Triangle>& triangles, const std::vector<size_t>& cell,
               std::vector<bool>& intersecting) {
    for (size_t i = 0; i < cell.size(); i++) {
        for (size_t j = i + 1; j < cell.size(); j++) {
            const size_t indexA = cell[i];
            const size_t indexB = cell[j];
            if (intersecting[indexA] && intersecting[indexB]) {
                continue;
            }

            if (triangles[indexA].HaveIntersection(triangles[indexB])) {
                intersecting[indexA] = intersecting[indexB] = true;
            }
        }
    }
}

} // namespace

std::vector<bool> FindIntersectingFlags(const std::vector<Triangle>& triangles) {
    std::vector<bool> intersecting(triangles.size(), false);
    if (triangles.size() < 2) {
        return intersecting;
    }

    const Grid grid = BuildGrid(triangles);
    const CellMap cells = BuildMap(triangles, grid);

    for (const auto& [key, cell] : cells) {
        CheckCell(triangles, cell, intersecting);
    }

    return intersecting;
}

} // namespace geometry
