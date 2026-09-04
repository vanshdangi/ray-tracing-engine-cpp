#pragma once
#include <core/ray.hpp>
#include <maths/point3.hpp>
#include <maths/vec3.hpp>

class Camera {
public:
    Camera(Point3 position_,
        Vec3 direction_,
        float fov_,
        float aspectRatio_,
        float imageWidth_,
        float imageHeight_);
    Ray generateRay(int pixelX, int pixelY, float sampleX, float sampleY) const;

private:
    Point3 position;
    Vec3 direction;

    Vec3 forward;
    Vec3 right;
    Vec3 up;

    float fov;
    float aspectRatio;

    float viewportWidth;
    float viewportHeight;
    float imageWidth;
    float imageHeight;
};