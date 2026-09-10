#include <world/objects/mesh.hpp>

Mesh::Mesh(Point3 position_, OBJData data_, Material mat_) : Object3D(mat_), position(position_), data(data_), triangles(data.triangles) {}

Intersection Mesh::intersects(const Ray& ray) const {
    Ray localRay;
    localRay.origin = Point3(ray.origin - position);
    localRay.direction = ray.direction;

    float closestT = std::numeric_limits<float>::infinity();
    const Triangle* closestObj = nullptr;

    for (const auto& triangle : triangles) {
        Intersection intersection = triangle.intersects(localRay);
        if (intersection.t >= 0.0f && intersection.t < closestT) {
            closestT = intersection.t;
            closestObj = &triangle;
        }
    }

    if (closestT == std::numeric_limits<float>::infinity()) {
        return {-1, nullptr};
    }
    return {closestT, closestObj};
}

Vec3 Mesh::getNormal(const Point3& point) const {
    return normal;
}

void Mesh::addTriangle(Point3 A, Point3 B, Point3 C) {
    triangles.emplace_back(Triangle(A, B, C, getMaterial()));
}