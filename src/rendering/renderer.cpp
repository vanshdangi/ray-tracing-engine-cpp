#include <iostream>
#include <render_stats.hpp>
#include <rendering/renderer.hpp>
#include <world/objects/sphere.hpp>
#include <world/hitRecord.hpp>
#include <thread>

Renderer::Renderer(const Camera& cam_, Image& img_, const Scene& scene_, const PointLight& light_)
    : cam(cam_), img(img_), scene(scene_), light(light_) {}

void Renderer::render() {
    renderStats.beginRender();

    unsigned int threadCount = std::thread::hardware_concurrency();

    if (threadCount == 0)
        threadCount = 4;

    threadCount = std::min<unsigned int>(threadCount, img.getHeight());

    if (threadCount == 0) {
        renderStats.finishRender();
        return;
    }

    std::vector<std::thread> threads;

    size_t rowsPerThread = img.getHeight() / threadCount;

    for (unsigned int i = 0; i < threadCount; ++i) {
        size_t startY = i * rowsPerThread;
        size_t endY = (i == threadCount - 1)
                    ? img.getHeight()
                    : startY + rowsPerThread;

        threads.emplace_back(&Renderer::renderRows, this, startY, endY);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    renderStats.finishRender();
}

void Renderer::renderRows(size_t startY, size_t endY) {

    for (size_t y = startY; y < endY; ++y) {

        for (size_t x = 0; x < img.getWidth(); ++x) {

            Color finalColor;

            for (size_t i = 0; i < 16; ++i) {
                Ray ray = cam.generateRay(
                    static_cast<int>(x), static_cast<int>(y), sampleX[i], sampleY[i]);
                renderStats.recordPrimaryRay();
                Color sampleColor = traceRay(ray, 0);
                finalColor += sampleColor;
            }

            renderStats.recordPixel();

            finalColor /= 16.0f;

            // clamp
            finalColor.r = std::min(1.0f, std::max(0.0f, finalColor.r));
            finalColor.g = std::min(1.0f, std::max(0.0f, finalColor.g));
            finalColor.b = std::min(1.0f, std::max(0.0f, finalColor.b));

            img.setPixel(static_cast<int>(x), static_cast<int>(y), finalColor);
        }
    }
}

Color Renderer::traceRay(const Ray& ray, int depth) const {
    renderStats.recordRay();
    auto hit = findClosestHit(ray);

    if (!hit) {
        return calculateBackground(ray);
    }

    Color local = calculateLighting(*hit, ray);

    const float reflectivity = hit->intersection.object->getMaterial().reflectivity;
    if (depth >= maxReflectionDepth || reflectivity <= 0.0f) {
        return local;
    }

    Color reflectedColor = calculateReflection(ray, *hit, depth);
    return local * (1 - reflectivity) + reflectedColor * reflectivity;
}

std::optional<HitRecord> Renderer::findClosestHit(const Ray& ray) const {
    float closestT = std::numeric_limits<float>::infinity();
    std::optional<Intersection> closestintersection;

    for(const auto& obj : scene.getObjects()) {
        Intersection interseciton = obj->intersects(ray);
        if(interseciton.t != -1 && interseciton.t < closestT) {
            closestT = interseciton.t;
            closestintersection = interseciton;
        }
    }
    if(!closestintersection) {
        return std::nullopt;
    }

    return HitRecord(ray.at(closestT), *closestintersection);
}

Color Renderer::calculateReflection(const Ray& ray, const HitRecord& hit, int depth) const {
    Ray reflectedRay;
    reflectedRay.direction = ray.direction - hit.normal*(2*(ray.direction.dot(hit.normal)));
    reflectedRay.origin = hit.point + hit.normal*(1e-5f);
    return traceRay(reflectedRay, depth + 1);
}

Color Renderer::calculateLighting(const HitRecord& hit, const Ray& ray) const {
    Color kd = hit.intersection.object->getMaterial().albedo;

    Vec3 normal = hit.intersection.object->getShadingNormal(hit.intersection, hit.point);

    // Orient the shading normal against the incoming ray.
    if (normal.dot(ray.direction) > 0.0f) {
        normal = normal * -1.0f;
    }

    // Ambient lighting (constant diffuse fill).
    constexpr float ambientIntensity = 0.15f;

    Color ambient(
        kd.r * ambientIntensity,
        kd.g * ambientIntensity,
        kd.b * ambientIntensity
    );

    // Direct lighting.
    Vec3 toLight = light.position - hit.point;
    float lightDistance = toLight.magnitude();
    Vec3 lightDirection = toLight.normalized();

    float diffuseFactor = light.intensity * std::max(0.0f, normal.dot(lightDirection));

    // Shadow
    Point3 shadowPoint = hit.point + normal * 1e-5f;

    Ray shadowRay;
    shadowRay.origin = shadowPoint;
    shadowRay.direction = lightDirection;

    renderStats.recordShadowRay();

    bool inShadow = false;

    for (const auto& obj : scene.getObjects()) {
        Intersection intersection = obj->intersects(shadowRay);

        if (intersection.t > 0.0f &&
            intersection.t < lightDistance) {
            inShadow = true;
            break;
        }
    }

    Color direct(0.0f, 0.0f, 0.0f);

    if (!inShadow) {
        direct = Color(
            kd.r * light.color.r * diffuseFactor,
            kd.g * light.color.g * diffuseFactor,
            kd.b * light.color.b * diffuseFactor
        );
    }

    return ambient + direct;
}

Color Renderer::calculateBackground(const Ray& ray) const {
    Color lDiffuse;

    Color top = Color::fromSrgb(0.275f, 0.588f, 0.863f);
    Color horizon = Color::fromSrgb(0.824f, 0.922f, 0.961f);
    Color ground = Color::fromSrgb(0.373f, 0.353f, 0.333f);

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