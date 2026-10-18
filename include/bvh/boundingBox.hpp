#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>
#include <world/object3D.hpp>
#include <world/transform.hpp>

class BoundingBox {
public:
    BoundingBox(Point3 min, Point3 max);
    float intersects(const Ray& ray) const;
    Point3 getCenter() const;
    Vec3 getHalfSize() const;

private:
    Point3 center = Point3(0, 0, 0);
    Vec3 halfSize;
};