#include <core/image.hpp>

void Image::setPixel(int x, int y, Color color) {
    pixels[y*width + x] = color;
}

Color Image::getPixel(int x, int y) const {
    return pixels[y*width + x];
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}