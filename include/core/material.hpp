#pragma once
#include <core/color.hpp>

struct Material {
    Color albedo;
    float metallic;
    float roughness;
    float reflectivity = 0.0f;    
};