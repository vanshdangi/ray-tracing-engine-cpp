#include <world/objects/triangle.hpp>

Triangle::Triangle(Point3 A_, Point3 B_, Point3 C_, Material mat_) : Object3D(mat_), A(A_), B(B_), C(C_) {
    // Centroid
    centroid.x = (A.x + B.x + C.x)/3.0f;
    centroid.y = (A.y + B.y + C.y)/3.0f;
    centroid.z = (A.z + B.z + C.z)/3.0f;

    // Normal
    normal = (B - A).cross(C - A).normalized();
}

Intersection Triangle::intersects(const Ray& ray) const {

    Vec3 E1 = (B - A);
    Vec3 E2 = (C - A);
    Vec3 T = ray.origin - A;
    float det = E1.dot(ray.direction.cross(E2));
    if(std::abs(det) < 1e-5f) {
        return {-1, this};
    }

    float u = T.dot(ray.direction.cross(E2)) / det;
    if(u < 0.0f || u > 1.0f) {
        return {-1, this};
    }

    Vec3 Q = T.cross(E1);
    float v = ray.direction.dot(Q) / det;
    if(v < 0.0f || u + v > 1.0f) {
        return {-1, this};
    }

    float t = E2.dot(Q) / det;
    if(t <= 0.0f) {
        return {-1, this};
    }

    return {t, this};
}

Vec3 Triangle::getNormal(const Point3& point) const {
    return normal;
}