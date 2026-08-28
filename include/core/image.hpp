#pragma once
#include <core/color.hpp>
#include <vector>

class Image {
public:
    Image(int width_, int height_) : width(width_), height(height_), pixels(width_ * height_) {}

    void setPixel(int x, int y, Color color);
    Color getPixel(int x, int y) const;

private:
    int width;
    int height;
    std::vector<Color> pixels;
};