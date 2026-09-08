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
    Color traceRay(const Ray& ray) const;
    std::optional<HitRecord> findClosestHit(const Ray& ray) const;
    Color calculateLighting(const HitRecord& hit) const;
    Color calculateBackground(const Ray& ray) const;
private:
    const Camera& cam;
    Image& img;
    const Scene& scene;
    const PointLight& light;

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