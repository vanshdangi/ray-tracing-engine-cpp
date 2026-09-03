#include <scene/sphere.hpp>

Sphere::Sphere(Point3 _center, float _raidus) : center(_center), radius(_raidus) {

}

float Sphere::intersects(const Ray& ray) {
    float a = ray.direction.magnitudeSquared();
    float b = 2*(ray.origin - center).dot(ray.direction);
    float c = (ray.origin - center).magnitudeSquared() - radius*radius;

    float disc = b*b - 4*a*c;
    if(disc < 0) {
        return -1;
    } else if(disc == 0) {
        float root = -b/(2*a);
        if(root < 0) {
            return -1;
        } else {
            return root;
        }
    } else {
        float root1 = (-b + std::sqrt(disc))/(2*a);
        float root2 = (-b - std::sqrt(disc))/(2*a);
        if(root1 < 0 && root2 < 0) {
            return -1;
        } else if(root1 < 0) {
            return root2;
        } else if(root2 < 0) {
            return root1;
        } else {
            return std::min(root1, root2);
        }
    }
}