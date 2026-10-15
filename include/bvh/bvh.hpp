#pragma once
#include <vector>
#include <bvh/boundingBox.hpp>
#include <world/objects/triangle.hpp>

struct BVHNode {
    BoundingBox boundingBox;

    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;

    std::vector<Triangle*> triangles;

    BVHNode(
        BoundingBox box,
        std::unique_ptr<BVHNode> leftChild,
        std::unique_ptr<BVHNode> rightChild,
        std::vector<Triangle*> triangles
    )
        : boundingBox(box),
          left(std::move(leftChild)),
          right(std::move(rightChild)),
          triangles(std::move(triangles)) {}
};