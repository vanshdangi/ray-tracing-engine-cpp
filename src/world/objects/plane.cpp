#include <world/objects/plane.hpp>
#include <maths/matrix4.hpp>

Plane::Plane(Transform transform_, Vec3 normal_, Material mat_) : Object3D(mat_), transform(transform_), normal(normal_) {}

Intersection Plane::intersects(const Ray& ray) const {
    Ray localRay = transform.toLocal(ray);

    float denominator = localRay.direction.dot(normal);

    if(std::abs(denominator) < 1e-5f) {
        return {-1, this, nullptr};
    }
    float t = (origin - localRay.origin).dot(normal)/denominator;
    if(t <= 0) {
        return {-1, this, nullptr};
    }
    return {t, this, nullptr};
}

Vec3 Plane::getNormal(const Point3& point) const {
    return normal;
}

Vec3 Plane::transformNormal(const Vec3& localNormal) const {
    return transform.transformNormal(localNormal);
}

Point3 Plane::getCenter() const {
    return origin;
}