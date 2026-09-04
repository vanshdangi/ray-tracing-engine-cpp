#include <rendering/renderer.hpp>
#include <iostream>
#include <world/objects/sphere.hpp>
#include <world/hitRecord.hpp>

Renderer::Renderer(const Camera& cam_, Image& img_, const Scene& scene_, const PointLight& light_)
    : cam(cam_), img(img_), scene(scene_), light(light_) {}

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
                Color lDiffuse;
    
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

                    
                    // Lighting Calculation
                    Color kd = hit.object.getMaterial().albedo;
                    Vec3 normal = hit.normal;
                    float diffuseFactor = light.intensity * std::max(0.0f, normal.dot((light.position - hit.point).normalized()));

                    // Shadow
                    Point3 shadowPoint = hit.point + hit.normal*(1e-5);
                    Ray shadowRay;
                    shadowRay.origin = shadowPoint;
                    shadowRay.direction = (light.position - hit.point).normalized();

                    bool inShadow = false;
                    for(const auto& obj : scene.getObjects()) {
                        float t = obj->intersects(shadowRay);
                        if(t != -1 && t < (light.position - hit.point).magnitude()) {
                            inShadow = true;
                            break;
                        }
                    }

                    if(inShadow) {
                        lDiffuse.r = 0;
                        lDiffuse.g = 0;
                        lDiffuse.b = 0;
                    } else {
                        lDiffuse.r = kd.r * light.color.r * diffuseFactor;
                        lDiffuse.g = kd.g * light.color.g * diffuseFactor;
                        lDiffuse.b = kd.b * light.color.b * diffuseFactor;
                    }
                } else {
                    // Background Gradient
                    Color top(70, 150, 220);
                    Color horizon(210, 235, 245);
                    Color ground(95, 90, 85);

                    if (ray.direction.x > 0.0f) {
                        float t = ray.direction.x;

                        lDiffuse.r = (1 - t) * horizon.r + t * top.r;
                        lDiffuse.g = (1 - t) * horizon.g + t * top.g;
                        lDiffuse.b = (1 - t) * horizon.b + t * top.b;
                    }
                    else {
                        float t = -ray.direction.x;

                        lDiffuse.r = (1 - t) * horizon.r + t * ground.r;
                        lDiffuse.g = (1 - t) * horizon.g + t * ground.g;
                        lDiffuse.b = (1 - t) * horizon.b + t * ground.b;
                    }
                }
                finalColor.r += lDiffuse.r;
                finalColor.g += lDiffuse.g;
                finalColor.b += lDiffuse.b;

            }
            finalColor.r /= 16.0f;
            finalColor.g /= 16.0f;
            finalColor.b /= 16.0f;

            //clamp
            finalColor.r = std::min(255.0f, std::max(0.0f, finalColor.r));
            finalColor.g = std::min(255.0f, std::max(0.0f, finalColor.g));
            finalColor.b = std::min(255.0f, std::max(0.0f, finalColor.b));
            img.setPixel(x, y, finalColor);
        }
    }
}