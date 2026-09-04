#include <world/lighting.hpp>

PointLight::PointLight(Point3 position_, float intensity_, Color color_) 
    : position(position_),  intensity(intensity_), color(color_) {}