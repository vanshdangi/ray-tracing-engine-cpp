#pragma once
#include <cmath>
#include <maths/vec3.hpp>
#include <maths/point3.hpp>

struct Ray {
    Point3 origin;
    Vec3 direction;

    Point3 at(float t) const {
        return origin + direction*t;
    }
};
