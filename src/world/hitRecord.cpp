#include <world/hitRecord.hpp>
#include <world/objects/triangle.hpp>

HitRecord::HitRecord(Point3 point_, const Intersection& intersection_) : t(intersection_.t), point(point_), intersection(intersection_) {
    if (intersection.primitive != nullptr) {
        Vec3 localNormal = intersection.primitive->getNormal(point);
        normal = intersection.object->transformNormal(localNormal);
    } else {
        Vec3 localNormal = intersection.object->getNormal(point);
        normal = intersection.object->transformNormal(localNormal);
    }
}