#pragma once

class Object3D;
class Triangle;
struct Intersection {
    float t = -1.0f;
    const Object3D* object = nullptr;
    const Triangle* primitive = nullptr;

    float u = 0.0f;
    float v = 0.0f;
};