#include <rendering/renderer.hpp>
#include <iostream>
#include <world/sphere.hpp>

Renderer::Renderer(const Camera& cam_, Image& img_, const Scene& scene_) : cam(cam_), img(img_), scene(scene_) {

}

void Renderer::render() {
    for(size_t y = 0; y < img.getHeight(); ++y){
        for(size_t x = 0; x < img.getWidth(); ++x){
            float closestT = std::numeric_limits<float>::infinity();

            Ray ray = cam.generateRay(x, y);
            Color color;

            for(const auto& obj : scene.getObjects()) {
                float t = obj.intersects(ray);
                if(t != -1 && t < closestT) {
                    Vec3 normal = (ray.at(t) - obj.getCenter()).normalized();
                    // Convert normal direction from [-1, 1] to [0, 255].
                    color.r = (normal.x + 1.0f) * 127.5f;
                    color.g = (normal.y + 1.0f) * 127.5f;
                    color.b = (normal.z + 1.0f) * 127.5f;

                    closestT = t;
                }
            }
            img.setPixel(x, y, color);
        }
    }
}