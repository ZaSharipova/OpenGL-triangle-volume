#include "input.hpp"

#include "triangle.hpp"

#include <iostream>
#include <string>
#include <optional>
#include <fstream>

namespace input {

namespace {
    std::optional<std::string> ParseCommandLine(int argc, char** argv) {
        if (argc == 1) {
            return std::string("");
        }

        if (argc > 3) {
            std::cerr << "Error: too many argv\n";
            return std::nullopt;
        }

        if (std::string(argv[1]) == "--file" && argv[2]) {
            return argv[2];
        }

        std::cerr << "error: invalid format of file input in command line\n";
        return std::nullopt;
    }

    std::istream* SelectInput(const std::string& path, std::ifstream& file) {
        if (path.empty()) {
            return &std::cin;
        }

        file.open(path);
        if (!file) {
            return nullptr;
        }

        return &file;
    }

    std::optional<std::vector<geometry::Triangle>> ReadTriangles(std::istream& in) {
        size_t size = 0;
        if (!(in >> size)) {
            std::cerr << "Failed to read size\n";
            return std::nullopt;
        }

        std::vector<geometry::Triangle> triangles;
        triangles.reserve(size);

        for (size_t i = 0; i < size; i++) {
            float line[9] {};
            for (float& value : line) {
                if (!(in >> value)) {
                    std::cerr << "Failed to read triangle #" + std::to_string(i);
                    return std::nullopt;
                }
            }

            triangles.emplace_back(geometry::Vec3(line[0], line[1], line[2]),
                                   geometry::Vec3(line[3], line[4], line[5]),
                                   geometry::Vec3(line[6], line[7], line[8]));
        }

        return triangles;
    }

} // namespace

std::optional<std::vector<geometry::Triangle>> LoadTriangles(int argc, char** argv) {
    std::optional<std::string> path = ParseCommandLine(argc, argv);
    if (!path.has_value()) {
        return std::nullopt;
    }

    std::ifstream file;
    std::istream* stream = SelectInput(path.value(), file);
    if (!stream) {
        std::cerr << "error: cannot open file\n";
        return std::nullopt;
    }

    return ReadTriangles(*stream);
}

} // namespace input
