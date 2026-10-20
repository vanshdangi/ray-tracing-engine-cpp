#pragma once

#include <world/scene.hpp>
#include <rendering/camera.hpp>
#include <world/lighting.hpp>

struct SceneConfig {
    Scene scene;
    Camera camera;
    PointLight light;
};

SceneConfig createBenchmarkScene(
    unsigned int width,
    unsigned int height
);
SceneConfig createBunnyScene(
    unsigned int width,
    unsigned int height
);
SceneConfig createReflectionsScene(
    unsigned int width,
    unsigned int height
);