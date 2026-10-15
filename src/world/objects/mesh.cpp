#include <iostream>
#include <render_stats.hpp>
#include <world/objects/mesh.hpp>
#include <maths/matrix4.hpp>
#include <algorithm>

Mesh::Mesh(Transform transform_, OBJData data_, Material mat_) : Object3D(mat_), transform(transform_), data(data_), triangles(data.triangles) {
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

    return traverseBVH(root.get(), localRay);
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

std::unique_ptr<BVHNode> Mesh::buildBVH(std::vector<Triangle*>& triangles){
    if (triangles.empty()) return nullptr;
    // Create Bounding Box
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float minZ = std::numeric_limits<float>::infinity();

    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();
    float maxZ = -std::numeric_limits<float>::infinity();

    for (Triangle* triangle : triangles) {
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
    if (triangles.size() <= 4) {
        return std::make_unique<BVHNode>(boundingBox, nullptr, nullptr, triangles);
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
    std::sort(triangles.begin(), triangles.end(),
        [axis](Triangle* a, Triangle* b) {
            Point3 ca = a->getCentroid();
            Point3 cb = b->getCentroid();

            if (axis == 0)
                return ca.x < cb.x;
            if (axis == 1)
                return ca.y < cb.y;

            return ca.z < cb.z;
        });
    size_t middle = triangles.size() / 2;
    left.assign(triangles.begin(), triangles.begin() + middle);
    right.assign(triangles.begin() + middle, triangles.end());
    
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

        for (const Triangle* triangle : node->triangles) {
            renderStats.recordTriangleTest();

            Intersection intersection = triangle->intersects(localRay);
            if (intersection.t >= 0.0f && intersection.t < closestT) {
                closestT = intersection.t;
                closestObj = triangle;
            }
        }

        if (closestT == std::numeric_limits<float>::infinity()) {
            return {-1,nullptr, nullptr};
        }
        return {closestT, this ,closestObj};
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