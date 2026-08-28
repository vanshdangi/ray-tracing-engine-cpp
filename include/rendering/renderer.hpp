#pragma once
#include <core/image.hpp>
#include <core/ray.hpp>
#include <rendering/camera.hpp>

class Renderer{
public:
    Renderer(const Camera& cam_, Image& img_);
    void render();
private:
    const Camera& cam;
    Image& img;
};