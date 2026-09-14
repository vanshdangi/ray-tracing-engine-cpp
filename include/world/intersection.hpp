#pragma once

class Object3D;
class Triangle;
struct Intersection {
    float t;
    const Object3D* object;
    const Triangle* primitive;
};