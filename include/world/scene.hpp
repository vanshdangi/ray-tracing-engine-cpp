#pragma once
#include <world/sphere.hpp>
#include <vector>

class Scene{
public:
    const std::vector<Sphere>& getObjects() const;
    void addObject(Sphere obj);
private:
    std::vector<Sphere> objects;
};