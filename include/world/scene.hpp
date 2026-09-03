#pragma once
#include <world/objects/sphere.hpp>
#include <world/object3D.hpp>
#include <vector>
#include <memory>

class Scene{
public:
    const std::vector<std::unique_ptr<Object3D>>& getObjects() const;
    void addObject(std::unique_ptr<Object3D> obj);
private:
    std::vector<std::unique_ptr<Object3D>> objects;
};