#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>

class Sphere{
public:
    Sphere(Point3 center_, float radius_);
    float intersects(const Ray& ray) const;
    Point3 getCenter() const;

private:
    Point3 center;
    float radius;
};