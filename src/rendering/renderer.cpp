#include <rendering/renderer.hpp>
#include <iostream>
#include <world/objects/sphere.hpp>
#include <world/hitRecord.hpp>

Renderer::Renderer(const Camera& cam_, Image& img_, const Scene& scene_) : cam(cam_), img(img_), scene(scene_) {

}

void Renderer::render() {
    for(size_t y = 0; y < img.getHeight(); ++y){
        for(size_t x = 0; x < img.getWidth(); ++x){
            Color finalColor;
            float sampleX[16] = {
                0.125f, 0.375f, 0.625f, 0.875f,
                0.125f, 0.375f, 0.625f, 0.875f,
                0.125f, 0.375f, 0.625f, 0.875f,
                0.125f, 0.375f, 0.625f, 0.875f
            };

            float sampleY[16] = {
                0.125f, 0.125f, 0.125f, 0.125f,
                0.375f, 0.375f, 0.375f, 0.375f,
                0.625f, 0.625f, 0.625f, 0.625f,
                0.875f, 0.875f, 0.875f, 0.875f
            };
            for (size_t i = 0; i < 16; i++)
            {
                float closestT = std::numeric_limits<float>::infinity();
                const Object3D* closestObj = nullptr;
                bool hitSomething = false;

                Ray ray = cam.generateRay(x, y, sampleX[i], sampleY[i]);
                Color color;
    
                for(const auto& obj : scene.getObjects()) {
                    float t = obj->intersects(ray);
                    if(t != -1 && t < closestT) {
                        closestT = t;
                        closestObj = obj.get();
                        hitSomething = true;
                    }
                }
                if (hitSomething) {
                    HitRecord hit(closestT, ray.at(closestT), *closestObj);

                    // Convert normal direction from [-1, 1] to [0, 255].
                    color.r = (hit.normal.x + 1.0f) * 127.5f;
                    color.g = (hit.normal.y + 1.0f) * 127.5f;
                    color.b = (hit.normal.z + 1.0f) * 127.5f;
                }
                finalColor.r += color.r;
                finalColor.g += color.g;
                finalColor.b += color.b;
            }
            finalColor.r /= 16.0f;
            finalColor.g /= 16.0f;
            finalColor.b /= 16.0f;
            img.setPixel(x, y, finalColor);
        }
    }
}