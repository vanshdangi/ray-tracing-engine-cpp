#include <world/objects/mesh.hpp>
#include <maths/matrix4.hpp>

Mesh::Mesh(Transform transform_, OBJData data_, Material mat_) : Object3D(mat_), transform(transform_), data(data_), triangles(data.triangles) {}

Intersection Mesh::intersects(const Ray& ray) const {
    Ray localRay = transform.toLocal(ray);

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
        return {-1,nullptr, nullptr};
    }
    return {closestT, this ,closestObj};
}

Vec3 Mesh::getNormal(const Point3& point) const {
    return normal;
}

Vec3 Mesh::transformNormal(const Vec3& localNormal) const {
    return transform.transformNormal(localNormal);
}

void Mesh::addTriangle(Point3 A, Point3 B, Point3 C) {
    triangles.emplace_back(Triangle(A, B, C, getMaterial()));
}