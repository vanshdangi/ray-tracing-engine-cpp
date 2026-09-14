#pragma once
#include <core/ray.hpp>
#include <core/material.hpp>
#include <maths/vec3.hpp>
#include <maths/point3.hpp>
#include <world/intersection.hpp>

class Object3D {
public:
    Object3D(Material mat_) : mat(mat_) {}
    virtual ~Object3D() = default;
    virtual Intersection intersects(const Ray& ray) const = 0;
    virtual Vec3 getNormal(const Point3& point) const = 0;
    virtual Vec3 transformNormal(const Vec3& localNormal) const = 0;

    void setMaterial(const Material& material) {
        mat = material;
    }

    const Material& getMaterial() const {
        return mat;
    }
private:
    Material mat;
};