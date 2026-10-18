#pragma once
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/ray.hpp>
#include <world/object3D.hpp>
#include <vector>
#include <world/objects/triangle.hpp>
#include <parser/objParser.hpp>
#include <world/transform.hpp>
#include <bvh/bvh.hpp>
#include <bvh/boundingBox.hpp>

class Mesh : public Object3D{
public:
    Mesh(Transform transform_, OBJData data_, Material mat_);
    Intersection intersects(const Ray& ray) const;
    Vec3 getNormal(const Point3& Point) const;
    Vec3 transformNormal(const Vec3& localNormal) const;

    Vec3 getShadingNormal(const Intersection& hit) const;
    Point2 getTexCoord(const Intersection& hit) const;
private:
    Transform transform;
    OBJData data;

    std::vector<Triangle> triangles;
    std::unique_ptr<BVHNode> root;
    Vec3 normal;

    std::unique_ptr<BVHNode> buildBVH(std::vector<Triangle*>& triangles);
    Intersection traverseBVH(const BVHNode* node, const Ray& localRay) const;
};