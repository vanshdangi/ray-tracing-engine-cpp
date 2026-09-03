#pragma once
#include <core/image.hpp>
#include <core/ray.hpp>
#include <rendering/camera.hpp>
#include <world/scene.hpp>

class Renderer{
public:
    Renderer(const Camera& cam_, Image& img_, const Scene& scene_);
    void render();
private:
    const Camera& cam;
    Image& img;
    const Scene& scene;
};