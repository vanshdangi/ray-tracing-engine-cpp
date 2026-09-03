#include <world/hitRecord.hpp>

HitRecord::HitRecord(float t_, Point3 point_, const Object3D& object_) : t(t_), point(point_), object(object_) {
    normal = object.getNormal(point);
}