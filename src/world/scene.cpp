#include <world/scene.hpp>

const std::vector<std::unique_ptr<Object3D>>& Scene::getObjects() const {
    return objects;
}

void Scene::addObject(std::unique_ptr<Object3D> obj){
    objects.push_back(std::move(obj));
}