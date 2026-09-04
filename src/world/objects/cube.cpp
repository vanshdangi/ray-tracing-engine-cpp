#include <world/objects/cube.hpp>

AABB::AABB(Point3 center_, float halfSize_) : center(center_), halfSize(halfSize_) {}

float AABB::intersects(const Ray& ray) const {
    // x-plane
    float minX = center.x - halfSize;
    float maxX = center.x + halfSize;

    float tx1 = (minX - ray.origin.x)/ray.direction.x;
    float tx2 = (maxX - ray.origin.x)/ray.direction.x;
    float txMin = std::min(tx1, tx2);
    float txMax = std::max(tx1, tx2);

    // y-plane
    float minY = center.y - halfSize;
    float maxY = center.y + halfSize;

    float ty1 = (minY - ray.origin.y)/ray.direction.y;
    float ty2 = (maxY - ray.origin.y)/ray.direction.y;
    float tyMin = std::min(ty1, ty2);
    float tyMax = std::max(ty1, ty2);

    // z-plane
    float minZ = center.z - halfSize;
    float maxZ = center.z + halfSize;

    float tz1 = (minZ - ray.origin.z)/ray.direction.z;
    float tz2 = (maxZ - ray.origin.z)/ray.direction.z;
    float tzMin = std::min(tz1, tz2);
    float tzMax = std::max(tz1, tz2);

    // Find Intersection
    float tMin = std::max({txMin, tyMin, tzMin});
    float tMax = std::min({txMax, tyMax, tzMax});

    if(tMin > tMax) {
        return -1;
    } else {
        if(tMax < 0) {
            return -1;
        } else if(tMin < 0) {
            return tMax;
        } else {
            return tMin;
        }
    }
}

bool areEqualAbsolute(double a, double b, double epsilon = 1e-5) {
    return std::abs(a - b) < epsilon;
}

Vec3 AABB::getNormal(const Point3& point) const {
    // x-plane
    float minX = center.x - halfSize;
    float maxX = center.x + halfSize;
    // y-plane
    float minY = center.y - halfSize;
    float maxY = center.y + halfSize;
    // z-plane
    float minZ = center.z - halfSize;
    float maxZ = center.z + halfSize;

    if(areEqualAbsolute(point.x, minX)){
        return Vec3(-1.0f, 0.0f, 0.0f);

    } else if(areEqualAbsolute(point.x, maxX)){
        return Vec3(1.0f, 0.0f, 0.0f);
        
    } else if(areEqualAbsolute(point.y, minY)){
        return Vec3(0.0f, -1.0f, 0.0f);

    } else if(areEqualAbsolute(point.y, maxY)){
        return Vec3(0.0f, 1.0f, 0.0f);

    } else if(areEqualAbsolute(point.z, minZ)){
        return Vec3(0.0f, 0.0f, -1.0f);

    } else if(areEqualAbsolute(point.z, maxZ)){
        return Vec3(0.0f, 0.0f, 1.0f);

    } else {
        return Vec3(0.0f, 0.0f, 0.0f);
    }
}



Point3 AABB::getCenter() const {
    return center;
}