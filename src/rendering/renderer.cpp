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
            for (size_t i = 0; i < 16; i++)
            {
                Ray ray = cam.generateRay(x, y, sampleX[i], sampleY[i]);
                Color sampleColor = traceRay(ray, 0);
                finalColor += sampleColor;

            }
            finalColor /= 16.0f;

            //clamp
            finalColor.r = std::min(255.0f, std::max(0.0f, finalColor.r));
            finalColor.g = std::min(255.0f, std::max(0.0f, finalColor.g));
            finalColor.b = std::min(255.0f, std::max(0.0f, finalColor.b));
            img.setPixel(x, y, finalColor);
        }
    }
}

Color Renderer::traceRay(const Ray& ray, int depth) const {
    auto hit = findClosestHit(ray);

    if (!hit) {
        return calculateBackground(ray);
    }

    Color local = calculateLighting(*hit);

    if (depth >= maxReflectionDepth) {
        return local;
    }

    Color reflectedColor = calculateReflection(ray, *hit, depth);
    float reflectivity = hit->object.getMaterial().reflectivity;
    return local * (1 - reflectivity) + reflectedColor * reflectivity;
}

std::optional<HitRecord> Renderer::findClosestHit(const Ray& ray) const {
    float closestT = std::numeric_limits<float>::infinity();
    const Object3D* closestObj = nullptr;

    for(const auto& obj : scene.getObjects()) {
        Intersection interseciton = obj->intersects(ray);
        if(interseciton.t != -1 && interseciton.t < closestT) {
            closestT = interseciton.t;
            closestObj = interseciton.object;
        }
    }
    if(closestObj == nullptr) {
        return std::nullopt;
    }

    return HitRecord(closestT, ray.at(closestT), *closestObj);
}

Color Renderer::calculateReflection(const Ray& ray, const HitRecord& hit, int depth) const {
    Ray reflectedRay;
    reflectedRay.direction = ray.direction - hit.normal*(2*(ray.direction.dot(hit.normal)));
    reflectedRay.origin = hit.point + hit.normal*(1e-5);
    return traceRay(reflectedRay, depth + 1);
}

Color Renderer::calculateLighting(const HitRecord& hit) const {
    Color lDiffuse;

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
        Intersection interseciton = obj->intersects(shadowRay);
        if(interseciton.t != -1 && interseciton.t < (light.position - hit.point).magnitude()) {
            inShadow = true;
            break;
        }
    }

    if(inShadow) {
        lDiffuse *= 0;
    } else {
        lDiffuse.r = kd.r * light.color.r * diffuseFactor;
        lDiffuse.g = kd.g * light.color.g * diffuseFactor;
        lDiffuse.b = kd.b * light.color.b * diffuseFactor;
    }

    return lDiffuse;
}

Color Renderer::calculateBackground(const Ray& ray) const {
    Color lDiffuse;

    Color top(70, 150, 220);
    Color horizon(210, 235, 245);
    Color ground(95, 90, 85);

    if (ray.direction.y > 0.0f) {
        float t = ray.direction.y;

        lDiffuse.r = (1 - t) * horizon.r + t * top.r;
        lDiffuse.g = (1 - t) * horizon.g + t * top.g;
        lDiffuse.b = (1 - t) * horizon.b + t * top.b;
    }
    else {
        float t = -ray.direction.y;

        lDiffuse.r = (1 - t) * horizon.r + t * ground.r;
        lDiffuse.g = (1 - t) * horizon.g + t * ground.g;
        lDiffuse.b = (1 - t) * horizon.b + t * ground.b;
    }

    return lDiffuse;
}