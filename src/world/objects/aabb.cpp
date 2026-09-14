#include <world/objects/aabb.hpp>
#include <algorithm>
#include <cmath>
#include <limits>
#include <maths/matrix4.hpp>

AABB::AABB(Transform transform_, float halfSize_, Material mat_) : Object3D(mat_), transform(transform_), halfSize(halfSize_) {}

Intersection AABB::intersects(const Ray& ray) const {
    Ray localRay = transform.toLocal(ray);

    float tMin = -std::numeric_limits<float>::infinity();
    float tMax = std::numeric_limits<float>::infinity();

    const auto updateSlab = [&](float origin, float direction, float minimum, float maximum) {
        if (direction == 0.0f) {
            return origin >= minimum && origin <= maximum;
        }

        float near = (minimum - origin) / direction;
        float far = (maximum - origin) / direction;
        if (near > far) {
            std::swap(near, far);
        }

        tMin = std::max(tMin, near);
        tMax = std::min(tMax, far);
        return tMin <= tMax;
    };

    if (!updateSlab(localRay.origin.x, localRay.direction.x, center.x - halfSize, center.x + halfSize) ||
        !updateSlab(localRay.origin.y, localRay.direction.y, center.y - halfSize, center.y + halfSize) ||
        !updateSlab(localRay.origin.z, localRay.direction.z, center.z - halfSize, center.z + halfSize)) {
        return {-1, this, nullptr};
    }

    if(tMin > tMax) {
        return {-1, this, nullptr};
    }

    float t = tMin < 0 ? tMax : tMin;
    if (t < 0) {
        return {-1, this, nullptr};
    }

    return {t, this, nullptr};
}

bool areEqualAbsolute(double a, double b, double epsilon = 1e-5) {
    return std::abs(a - b) < epsilon;
}

Vec3 AABB::getNormal(const Point3& point) const {
    Point3 local = transform.toLocal({point, Vec3(0.0f, 0.0f, 0.0f)}).origin;

    // x-plane
    float minX = center.x - halfSize;
    float maxX = center.x + halfSize;
    // y-plane
    float minY = center.y - halfSize;
    float maxY = center.y + halfSize;
    // z-plane
    float minZ = center.z - halfSize;
    float maxZ = center.z + halfSize;

    if(areEqualAbsolute(local.x, minX)){
        return Vec3(-1.0f, 0.0f, 0.0f);

    } else if(areEqualAbsolute(local.x, maxX)){
        return Vec3(1.0f, 0.0f, 0.0f);
        
    } else if(areEqualAbsolute(local.y, minY)){
        return Vec3(0.0f, -1.0f, 0.0f);

    } else if(areEqualAbsolute(local.y, maxY)){
        return Vec3(0.0f, 1.0f, 0.0f);

    } else if(areEqualAbsolute(local.z, minZ)){
        return Vec3(0.0f, 0.0f, -1.0f);

    } else if(areEqualAbsolute(local.z, maxZ)){
        return Vec3(0.0f, 0.0f, 1.0f);

    } else {
        return Vec3(0.0f, 0.0f, 0.0f);
    }
}

Vec3 AABB::transformNormal(const Vec3& localNormal) const {
    return transform.transformNormal(localNormal);
}


Point3 AABB::getCenter() const {
    return transform.toWorld(center);
}