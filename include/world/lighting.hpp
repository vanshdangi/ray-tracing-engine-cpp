#pragma once
#include <maths/point3.hpp>
#include <core/color.hpp>

struct PointLight{
    Point3 position;
    float intensity;
    Color color;

    PointLight(Point3 position_, float intensity_, Color color_);
};