#include <rendering/camera.hpp>
#include <cmath>
#include <numbers>

Camera::Camera(Point3 position_,
    Vec3 direction_,
    float fov_,
    float aspectRatio_,
    float imageWidth_,
    float imageHeight_
) : position(position_),
 direction(direction_),
 fov(fov_),
 aspectRatio(aspectRatio_),
 imageWidth(imageWidth_),
 imageHeight(imageHeight_) {

    forward = direction.normalized();
    Vec3 worldUp(0.0f, 1.0f, 0.0f);
    if(std::abs(forward.dot(worldUp)) > 0.999f){
        worldUp = Vec3(1.0f, 0.0f, 0.0f);
    }
    right = forward.cross(worldUp).normalized();
    up = right.cross(forward).normalized();

    float fovRadians = fov * std::numbers::pi_v<float> / 180.0f;
    viewportHeight = 2*std::tan(fovRadians/2);
    viewportWidth = viewportHeight*aspectRatio;
}

Ray Camera::generateRay(int pixelX, int pixelY, float sampleX, float sampleY) const {
    float pixelWidth = viewportWidth/imageWidth;
    float pixelHeight = viewportHeight/imageHeight;
    float xOffset = (pixelX + sampleX)*pixelWidth - viewportWidth/2.0f;
    float yOffset = viewportHeight/2.0f - (pixelY + sampleY)*pixelHeight;

    Point3 center = position + forward;
    Point3 pointOnViewPort = center + right*xOffset + up*yOffset;

    Ray finalRay;
    finalRay.origin = position;
    finalRay.direction = (pointOnViewPort - position).normalized();

    return finalRay;
}