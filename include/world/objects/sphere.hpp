#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>
#include <world/object3D.hpp>

class Sphere : public Object3D{
public:
    Sphere(Point3 center_, float radius_, Material mat_);
    float intersects(const Ray& ray) const;
    Vec3 getNormal(const Point3& point) const;
    Point3 getCenter() const;

private:
    Point3 center;
    float radius;
};