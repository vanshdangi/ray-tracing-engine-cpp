#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>

class Sphere{
public:
    Sphere(Point3 _center, float _radius);
    float intersects(const Ray& ray);

private:
    Point3 center;
    float radius;
};