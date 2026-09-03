#pragma once
#include <world/scene.hpp>
#include <world/objects/sphere.hpp>
#include <maths/point3.hpp>
#include <maths/vec3.hpp>

struct HitRecord {
    float t;
    Point3 point;
    Vec3 normal;
    const Object3D& object;

    HitRecord(float t_, Point3 point_, const Object3D& object_);
};