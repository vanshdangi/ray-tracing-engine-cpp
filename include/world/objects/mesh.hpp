#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>
#include <world/object3D.hpp>
#include <vector>
#include <world/objects/triangle.hpp>

class Mesh : public Object3D{
public:
    Mesh(std::vector<Triangle> triangles_, Material mat_);
    Intersection intersects(const Ray& ray) const;
    Vec3 getNormal(const Point3& Point) const;

private:
    Point3 A;
    Point3 B;
    Point3 C;

    std::vector<Triangle> triangles;

    Point3 centroid;
    Vec3 normal;
};