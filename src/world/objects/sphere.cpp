#include <world/objects/sphere.hpp>
#include <algorithm>
#include <maths/matrix4.hpp>

Sphere::Sphere(Transform transform_, float raidus_, Material mat_) : Object3D(mat_), transform(transform_), radius(raidus_) {}

Intersection Sphere::intersects(const Ray& ray) const {
    Ray localRay = transform.toLocal(ray);

    float a = localRay.direction.magnitudeSquared();
    float b = 2*(localRay.origin - center).dot(localRay.direction);
    float c = (localRay.origin - center).magnitudeSquared() - radius*radius;

    float disc = b*b - 4*a*c;
    if(disc < 0) {
        return {-1, this};
    } else if(disc == 0) {
        float root = -b/(2*a);
        if(root < 0) {
            return {-1, this};
        } else {
            return {root, this};
        }
    } else {
        float root1 = (-b + std::sqrt(disc))/(2*a);
        float root2 = (-b - std::sqrt(disc))/(2*a);
        if(root1 < 0 && root2 < 0) {
            return {-1, this};
        } else if(root1 < 0) {
            return {root2, this};
        } else if(root2 < 0) {
            return {root1, this};
        } else {
            return {std::min(root1, root2), this};
        }
    }
}

Vec3 Sphere::getNormal(const Point3& point) const {
    Point3 local = transform.toLocal({point, Vec3(0.0f, 0.0f, 0.0f)}).origin;
    return (local - center).normalized();
}

Vec3 Sphere::transformNormal(const Vec3& localNormal) const {
    return transform.transformNormal(localNormal);
}

Point3 Sphere::getCenter() const {
    return center;
}