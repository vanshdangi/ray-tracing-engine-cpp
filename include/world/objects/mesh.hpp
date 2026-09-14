#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>
#include <world/object3D.hpp>
#include <vector>
#include <world/objects/triangle.hpp>
#include <parser/objParser.hpp>
#include <world/transform.hpp>

class Mesh : public Object3D{
public:
    Mesh(Transform transform_, OBJData data_, Material mat_);
    Intersection intersects(const Ray& ray) const;
    Vec3 getNormal(const Point3& Point) const;
    Vec3 transformNormal(const Vec3& localNormal) const;
    void addTriangle(Point3 A, Point3 B, Point3 C);

private:
    Transform transform;
    OBJData data;

    std::vector<Triangle> triangles;

    Point3 centroid;
    Vec3 normal;
};