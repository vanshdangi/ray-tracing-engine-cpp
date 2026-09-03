#include <world/scene.hpp>

const std::vector<Sphere>& Scene::getObjects() const {
    return objects;
}

void Scene::addObject(Sphere obj){
    objects.push_back(obj);
}