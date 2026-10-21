#include <iostream>
#include <render_stats.hpp>
#include <world/objects/mesh.hpp>
#include <maths/matrix4.hpp>
#include <algorithm>
#include <cmath>

Mesh::Mesh(Transform transform_, OBJData data_, Material mat_) : Object3D(mat_), transform(transform_), data(std::move(data_)), triangles(data.triangles) {
    std::cout << "Total Triangles: " << triangles.size() << '\n';

    // Create BVH
    std::vector<Triangle*> trianglePointers;
    trianglePointers.reserve(triangles.size());
    for (Triangle& triangle : triangles) {
        trianglePointers.push_back(&triangle);
    }
    root = buildBVH(trianglePointers);
}

Intersection Mesh::intersects(const Ray& ray) const {
    Ray localRay = transform.toLocal(ray);

    if (bvhEnabled) {
        return traverseBVH(root.get(), localRay);
    }

    float closestT = std::numeric_limits<float>::infinity();
    const Triangle* closestObj = nullptr;
    float closestU = 0.0f;
    float closestV = 0.0f;

    for (const Triangle& triangle : triangles) {
        const Intersection intersection = triangle.intersects(localRay);
        if (intersection.t >= 0.0f && intersection.t < closestT) {
            closestT = intersection.t;
            closestObj = &triangle;
            closestU = intersection.u;
            closestV = intersection.v;
        }
    }
    renderStats.recordTriangleTests(triangles.size());

    if (closestObj == nullptr) {
        return {-1.0f, nullptr, nullptr};
    }
    return {closestT, this, closestObj, closestU, closestV};
}

Vec3 Mesh::getNormal(const Point3&) const {
    return normal;
}

Vec3 Mesh::transformNormal(const Vec3& localNormal) const {
    return transform.transformNormal(localNormal);
}


std::unique_ptr<BVHNode> Mesh::buildBVH(std::vector<Triangle*>& trianglePointers){
    if (trianglePointers.empty()) return nullptr;
    // Create Bounding Box
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float minZ = std::numeric_limits<float>::infinity();

    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();
    float maxZ = -std::numeric_limits<float>::infinity();

    for (Triangle* triangle : trianglePointers) {
        const Point3 vertices[] = {
            triangle->getA(),
            triangle->getB(),
            triangle->getC()
        };

        for (const auto& vertex : vertices) {
            minX = std::min(minX, vertex.x);
            maxX = std::max(maxX, vertex.x);

            minY = std::min(minY, vertex.y);
            maxY = std::max(maxY, vertex.y);

            minZ = std::min(minZ, vertex.z);
            maxZ = std::max(maxZ, vertex.z);
        }
    }
    BoundingBox boundingBox(Point3(minX, minY, minZ), Point3(maxX, maxY, maxZ));
    if (trianglePointers.size() <= 4) {
        return std::make_unique<BVHNode>(boundingBox, nullptr, nullptr, trianglePointers);
    }

    // Split Triangles along longest axis
    std::vector<Triangle*> right;
    std::vector<Triangle*> left;

    float width  = maxX - minX;
    float height = maxY - minY;
    float depth  = maxZ - minZ ;

    int axis = (width >= height && width >= depth) ? 0
                : (height >= width && height >= depth) ? 1
                : 2;
    std::sort(trianglePointers.begin(), trianglePointers.end(),
        [axis](Triangle* a, Triangle* b) {
            Point3 ca = a->getCentroid();
            Point3 cb = b->getCentroid();

            if (axis == 0)
                return ca.x < cb.x;
            if (axis == 1)
                return ca.y < cb.y;

            return ca.z < cb.z;
        });
    size_t middle = trianglePointers.size() / 2;
    left.assign(trianglePointers.begin(), trianglePointers.begin() + middle);
    right.assign(trianglePointers.begin() + middle, trianglePointers.end());
    
    // Recursively build the two children
    auto leftNode = buildBVH(left);
    auto rightNode = buildBVH(right);

    // Create and return the internal node
    return std::make_unique<BVHNode>(
        boundingBox, std::move(leftNode), std::move(rightNode), std::vector<Triangle*>{}
    );
}

Intersection Mesh::traverseBVH(const BVHNode* node, const Ray& localRay) const {
    // if node is null
    if (node == nullptr) {
        return {-1, nullptr, nullptr};
    }
    // if object misses the node
    if (node->boundingBox.intersects(localRay) < 0.0f) {
        return {-1, nullptr, nullptr};
    }

    // if leaf node
    if (!node->triangles.empty()) {
        float closestT = std::numeric_limits<float>::infinity();
        const Triangle* closestObj = nullptr;
        float closestU = 0.0f;
        float closestV = 0.0f;

        for (const Triangle* triangle : node->triangles) {
            Intersection intersection = triangle->intersects(localRay);
            if (intersection.t >= 0.0f && intersection.t < closestT) {
                closestT = intersection.t;
                closestObj = triangle;
                closestU = intersection.u;
                closestV = intersection.v;
            }
        }
        renderStats.recordTriangleTests(node->triangles.size());

        if (closestT == std::numeric_limits<float>::infinity()) {
            return {-1,nullptr, nullptr};
        }
        return {closestT, this, closestObj, closestU, closestV};
    }

    // if not leaf node
    Intersection leftHit = traverseBVH(node->left.get(), localRay);
    Intersection rightHit = traverseBVH(node->right.get(), localRay);

    if (leftHit.t == -1 && rightHit.t == -1) {
        return {-1, nullptr, nullptr};
    } else if (leftHit.t >= 0 && rightHit.t == -1) {
        return leftHit;
    } else if (leftHit.t == -1 && rightHit.t >= 0) {
        return rightHit;
    } else {
        if (leftHit.t <= rightHit.t) {
            return leftHit;
        } else {
            return rightHit;
        }
    }
}

Vec3 Mesh::getShadingNormal(const Intersection& hit, const Point3&) const {
    const auto* triangle =
        static_cast<const Triangle*>(hit.primitive);

    const std::size_t index =
        static_cast<std::size_t>(triangle - triangles.data());

    if (index >= data.triangleIndices.size()) {
        Vec3 localNormal = triangle->getNormal(Point3());
        return transform.transformNormal(localNormal).normalized();
    }
    const auto& corners = data.triangleIndices[index].vertices;

    // If any normal is absent, fall back to the flat triangle normal.
    if (corners[0].normal < 0 ||
        corners[1].normal < 0 ||
        corners[2].normal < 0) {
        return transform.transformNormal(triangle->getNormal(Point3()));
    }

    const Vec3& nA = data.normals[corners[0].normal];
    const Vec3& nB = data.normals[corners[1].normal];
    const Vec3& nC = data.normals[corners[2].normal];

    const float u = hit.u;
    const float v = hit.v;
    const float w = 1.0f - u - v;

    Vec3 interpolatedNormal = (
        nA * w +
        nB * u +
        nC * v
    );

    if (!std::isfinite(interpolatedNormal.magnitudeSquared()) ||
        interpolatedNormal.magnitudeSquared() == 0.0f) {
        Vec3 localNormal = triangle->getNormal(Point3());
        return transform.transformNormal(localNormal).normalized();
    }

    return transform.transformNormal(interpolatedNormal.normalized()).normalized();
}

Point2 Mesh::getTexCoords(const Intersection& hit) const {
    const auto* triangle =
        static_cast<const Triangle*>(hit.primitive);

    const std::size_t index =
        static_cast<std::size_t>(triangle - triangles.data());

    const auto& corners = data.triangleIndices[index].vertices;

    if (corners[0].texCoord < 0 ||
        corners[1].texCoord < 0 ||
        corners[2].texCoord < 0) {
        return Point2(0.0f, 0.0f);
    }

    const Point2& uvA = data.texCoords[corners[0].texCoord];
    const Point2& uvB = data.texCoords[corners[1].texCoord];
    const Point2& uvC = data.texCoords[corners[2].texCoord];

    const float u = hit.u;
    const float v = hit.v;
    const float w = 1.0f - u - v;

    return Point2(
        w * uvA.x + u * uvB.x + v * uvC.x,
        w * uvA.y + u * uvB.y + v * uvC.y
    );
}