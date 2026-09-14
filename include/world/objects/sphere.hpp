#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>
#include <world/object3D.hpp>
#include <world/transform.hpp>

class Sphere : public Object3D{
public:
    Sphere(Transform transform_, float radius_, Material mat_);
    Intersection intersects(const Ray& ray) const;
    Vec3 getNormal(const Point3& point) const;
    Vec3 transformNormal(const Vec3& localNormal) const;
    Point3 getCenter() const;

private:
    Point3 center = Point3(0.0f, 0.0f, 0.0f);
    Transform transform;
    float radius;
};