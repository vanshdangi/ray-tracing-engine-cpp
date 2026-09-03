#include <rendering/renderer.hpp>
#include <iostream>
#include <scene/sphere.hpp>

Renderer::Renderer(const Camera& cam_, Image& img_) : cam(cam_), img(img_) {

}

void Renderer::render() {
    Point3 center(0.0f, 5.0f, 0.0f);
    Sphere sphereObj(center, 2);

    for(size_t y = 0; y < img.getHeight(); ++y){
        for(size_t x = 0; x < img.getWidth(); ++x){
            Ray ray = cam.generateRay(x, y);

            float t = sphereObj.intersects(ray);
            
            Color color;
            if(t != -1) {
                Vec3 normal = (ray.at(t) - center).normalized();
                // Convert normal direction from [-1, 1] to [0, 255].
                color.r = (normal.x + 1.0f) * 127.5f;
                color.g = (normal.y + 1.0f) * 127.5f;
                color.b = (normal.z + 1.0f) * 127.5f;
            }
            img.setPixel(x, y, color);
        }
    }
}