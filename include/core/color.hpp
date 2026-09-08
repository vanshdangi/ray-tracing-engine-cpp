#pragma once

struct Color {
    float r, g, b;

    Color() : r(0.0f), g(0.0f), b(0.0f) {}
    Color(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}

    bool operator==(const Color& other) const {
        return r == other.r &&
               g == other.g &&
               b == other.b;
    }

    Color operator+(const Color& other) const {
        return { r + other.r, g + other.g, b + other.b };
    }

    Color operator-(const Color& other) const {
        return { r - other.r, g - other.g, b - other.b };
    }

    Color operator*(float scalar) const {
        return { r * scalar, g * scalar, b * scalar };
    }

    Color operator/(float scalar) const {
        return { r / scalar, g / scalar, b / scalar };
    }

    Color& operator+=(const Color& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    Color& operator-=(const Color& other) {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        return *this;
    }

    Color& operator*=(float scalar) {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        return *this;
    }

    Color& operator/=(float scalar) {
        r /= scalar;
        g /= scalar;
        b /= scalar;
        return *this;
    }

};