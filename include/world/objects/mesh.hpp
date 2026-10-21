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
    static void setBVHEnabled(bool enabled) { bvhEnabled = enabled; }
    Vec3 getNormal(const Point3&) const;
    Vec3 transformNormal(const Vec3& localNormal) const;

    Vec3 getShadingNormal(const Intersection& hit, const Point3& point) const override;
    Point2 getTexCoords(const Intersection& hit) const override;
private:
    Transform transform;
    OBJData data;

    std::vector<Triangle> triangles;
    std::unique_ptr<BVHNode> root;
    Vec3 normal;
    inline static bool bvhEnabled = true;

    std::unique_ptr<BVHNode> buildBVH(std::vector<Triangle*>& triangles);
    Intersection traverseBVH(const BVHNode* node, const Ray& localRay) const;
};