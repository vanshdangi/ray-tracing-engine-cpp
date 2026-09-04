#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>
#include <world/object3D.hpp>

class Plane : public Object3D{
public:
    Plane(Point3 origin_, Vec3 normal_, Material mat_);
    float intersects(const Ray& ray) const;
    Vec3 getNormal(const Point3& point) const;
    Point3 getCenter() const;

private:
    Point3 origin;
    Vec3 normal;
};