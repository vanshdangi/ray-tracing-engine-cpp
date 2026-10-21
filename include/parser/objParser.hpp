
#pragma once

#include <array>
#include <vector>
#include <string>

#include <world/objects/triangle.hpp>
#include <maths/point3.hpp>
#include <maths/point2.hpp>

struct OBJVertexIndex {
    int vertex = -1;
    int texCoord = -1;
    int normal = -1;
};

struct OBJTriangleIndices {
    std::array<OBJVertexIndex, 3> vertices;
};

struct OBJData {
    std::vector<Point3> vertices;             // v
    std::vector<Point2> texCoords;            // vt
    std::vector<Vec3> normals;                // vn

    std::vector<Triangle> triangles;           // Renderable geometry
    std::vector<OBJTriangleIndices> triangleIndices; // Per-corner attributes
};

class OBJParser {
public:
    OBJData parser(const std::string& filename);
};

int parseOBJIndex(int index, std::size_t count);
OBJData loadOBJ(const std::string& relativePath);