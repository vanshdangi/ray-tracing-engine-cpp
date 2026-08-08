#pragma once
#include <cmath>

struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator+(const Vec3& other) const {
        return { x + other.x, y + other.y, z + other.z};
    }
    Vec3 operator-(const Vec3& other) const {
        return { x - other.x, y - other.y, z - other.z};
    }
    Vec3 operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar};
    }
    Vec3 operator/(float scalar) const {
        return { x / scalar, y / scalar, z / scalar};
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vec3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    Vec3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    float magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    float magnitudeSquared() const {
        return x*x + y*y + z*z;
    }

    Vec3 normalized() const {
        float len = magnitude();
        if (len == 0.0f) return {0, 0, 0};
        return *this / len;
    }

    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 cross(const Vec3& other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }
};
