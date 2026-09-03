#pragma once
#include <core/ray.hpp>
#include <core/material.hpp>
#include <maths/vec3.hpp>
#include <maths/point3.hpp>

class Object3D {
public:
    virtual ~Object3D() = default;
    virtual float intersects(const Ray& ray) const = 0;
    virtual Vec3 getNormal(const Point3& Point) const = 0;

    void setMaterial(const Material& material) {
        mat = material;
    }

    const Material& getMaterial() const {
        return mat;
    }
private:
    Material mat;
};