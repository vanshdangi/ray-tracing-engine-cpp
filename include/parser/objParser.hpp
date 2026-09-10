#pragma once
#include <vector>
#include <string>
#include <world/objects/triangle.hpp>
#include <maths/point3.hpp>

struct OBJData {
    std::vector<Point3> vertices;
    std::vector<Triangle> triangles;
};

class OBJParser{
public:
    OBJData parser(const std::string& filename);
private:
    OBJData data;
};