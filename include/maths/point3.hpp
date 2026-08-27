#pragma once
#include <cmath>
#include <maths/vec3.hpp>

struct Point3 {
    float x, y, z;

    Point3() : x(0), y(0), z(0) {}
    Point3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    bool operator==(const Point3& other) const {
        return x == other.x &&
               y == other.y &&
               z == other.z;
    }

    Vec3 operator-(const Point3& other) const {
        return { x - other.x, y - other.y, z - other.z};
    }

    Point3 operator+(const Vec3& other) const {
        return { x + other.x, y + other.y, z + other.z};
    }
    Point3 operator-(const Vec3& other) const {
        return { x - other.x, y - other.y, z - other.z};
    }

    Point3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Point3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
};
