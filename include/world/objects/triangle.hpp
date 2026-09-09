#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>
#include <world/object3D.hpp>

class Triangle : public Object3D{
public:
    Triangle(Point3 A_, Point3 B_, Point3 C_, Material mat_);
    Intersection intersects(const Ray& ray) const;
    Vec3 getNormal(const Point3& Point) const;

private:
    Point3 A;
    Point3 B;
    Point3 C;

    Point3 centroid;
    Vec3 normal;
};