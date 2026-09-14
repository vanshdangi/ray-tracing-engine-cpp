#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>
#include <world/object3D.hpp>
#include <world/transform.hpp>

class Plane : public Object3D{
public:
    Plane(Transform transform_, Vec3 normal_, Material mat_);
    Intersection intersects(const Ray& ray) const;
    Vec3 getNormal(const Point3& point) const;
    Vec3 transformNormal(const Vec3& localNormal) const;
    Point3 getCenter() const;

private:
    Point3 origin = Point3(0.0f, 0.0f, 0.0f);
    Vec3 normal;
    Transform transform;
};