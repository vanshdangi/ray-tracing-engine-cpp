#pragma once
#include <world/scene.hpp>
#include <world/objects/sphere.hpp>
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <world/object3D.hpp>

struct HitRecord {
    float t;
    Point3 point;
    Vec3 normal;
    Intersection intersection;

    HitRecord(Point3 point_, const Intersection& intersection_);
};