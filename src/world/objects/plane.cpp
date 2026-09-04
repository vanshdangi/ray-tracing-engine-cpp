#include <world/objects/plane.hpp>

Plane::Plane(Point3 origin_, Vec3 normal_, Material mat_) : Object3D(mat_), origin(origin_), normal(normal_) {}

float Plane::intersects(const Ray& ray) const {
    float denominator = ray.direction.dot(normal);

    if(std::abs(denominator) < 1e-5f) {
        return -1;
    }
    float t = (origin - ray.origin).dot(normal)/denominator;
    if(t <= 0) {
        return -1;
    }
    return t;
}

Vec3 Plane::getNormal(const Point3& point) const {
    return normal;
}

Point3 Plane::getCenter() const {
    return origin;
}