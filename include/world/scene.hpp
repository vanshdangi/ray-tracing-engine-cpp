#pragma once
#include <world/objects/sphere.hpp>
#include <world/object3D.hpp>
#include <world/lighting.hpp>
#include <rendering/camera.hpp>
#include <vector>
#include <memory>


class Scene{
    public:
    const std::vector<std::unique_ptr<Object3D>>& getObjects() const;
    void addObject(std::unique_ptr<Object3D> obj);
    private:
    std::vector<std::unique_ptr<Object3D>> objects;
};

struct SceneConfig {
    Scene scene;
    Camera camera;
    PointLight light;
    float ambientIntensity = 0.15f;
};