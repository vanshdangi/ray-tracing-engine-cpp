#pragma once
#include <core/ray.hpp>
#include <maths/vec3.hpp>
#include <maths/point3.hpp>
#include <maths/matrix4.hpp>

class Transform {
public:
    Point3 position;
    Vec3 rotation;
    Vec3 scale;

    Transform();
    Transform(Point3 position_, Vec3 rotation_, Vec3 scale_);

    Matrix4 getMatrix() const;
    Matrix4 getInverseMatrix() const;
    
    Ray toLocal(const Ray& worldRay) const;
    Point3 toWorld(const Point3& localPoint) const;
    Vec3 transformNormal(const Vec3& localNormal) const;
};