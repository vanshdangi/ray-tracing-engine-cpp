#pragma once
#include <core/image.hpp>
#include <core/ray.hpp>
#include <rendering/camera.hpp>
#include <world/scene.hpp>
#include <world/lighting.hpp>
#include <world/hitRecord.hpp>
#include <optional>

class Renderer{
public:
    Renderer(const Camera& cam_, Image& img_, const Scene& scene_, const PointLight& light_);
    void render();
    void renderRows(size_t startY, size_t endY);
    Color traceRay(const Ray& ray, int depth) const;
    std::optional<HitRecord> findClosestHit(const Ray& ray) const;
    Color calculateReflection(const Ray& ray, const HitRecord& hit, int depth) const;
    Color calculateLighting(const HitRecord& hit) const;
    Color calculateBackground(const Ray& ray) const;
private:
    const Camera& cam;
    Image& img;
    const Scene& scene;
    const PointLight& light;
    const int maxReflectionDepth = 3;

    static constexpr float sampleX[16] = {
        0.125f, 0.375f, 0.625f, 0.875f,
        0.125f, 0.375f, 0.625f, 0.875f,
        0.125f, 0.375f, 0.625f, 0.875f,
        0.125f, 0.375f, 0.625f, 0.875f
    };
    static constexpr float sampleY[16] = {
        0.125f, 0.125f, 0.125f, 0.125f,
        0.375f, 0.375f, 0.375f, 0.375f,
        0.625f, 0.625f, 0.625f, 0.625f,
        0.875f, 0.875f, 0.875f, 0.875f
    };
};