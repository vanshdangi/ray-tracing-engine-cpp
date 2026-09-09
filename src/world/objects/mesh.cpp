#include <world/objects/mesh.hpp>

Mesh::Mesh(std::vector<Triangle> triangles_, Material mat_) : Object3D(mat_), triangles(triangles_) {}

Intersection Mesh::intersects(const Ray& ray) const {
    float closestT = std::numeric_limits<float>::infinity();
    const Triangle* closestObj = nullptr;

    for (const auto& triangle : triangles) {
        Intersection intersection = triangle.intersects(ray);
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