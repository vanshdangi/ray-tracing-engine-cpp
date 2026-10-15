#include <algorithm>
#include <limits>
#include <bvh/boundingBox.hpp>

BoundingBox::BoundingBox(Point3 min, Point3 max) : 
    halfSize((max - min)/2) {
        center = Point3(
            (min.x + max.x)/2,
            (min.y + max.y)/2,
            (min.z + max.z)/2
        );
    }

float BoundingBox::intersects(const Ray& ray) const {

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

    if (!updateSlab(ray.origin.x, ray.direction.x, center.x - halfSize.x, center.x + halfSize.x) ||
        !updateSlab(ray.origin.y, ray.direction.y, center.y - halfSize.y, center.y + halfSize.y) ||
        !updateSlab(ray.origin.z, ray.direction.z, center.z - halfSize.z, center.z + halfSize.z)) {
        return -1;
    }

    if(tMin > tMax) {
        return -1;
    }

    float t = tMin < 0 ? tMax : tMin;
    if (t < 0) {
        return -1;
    }

    return t;
}

Point3 BoundingBox::getCenter() const {
    return center;
}