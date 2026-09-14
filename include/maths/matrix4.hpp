#pragma once

#include <array>
#include <cmath>
#include <stdexcept>
#include <utility>

#include <maths/point3.hpp>
#include <maths/vec3.hpp>

class Matrix4 {
public:
    Matrix4() : values_({
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }) {}

    explicit Matrix4(const std::array<float, 16>& values) : values_(values) {}

    Matrix4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    ) : values_({
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33
    }) {}

    static Matrix4 identity() {
        return Matrix4();
    }

    static Matrix4 zero() {
        return Matrix4(std::array<float, 16>{});
    }

    float& operator()(int row, int column) {
        return values_[static_cast<std::size_t>(row * 4 + column)];
    }

    float operator()(int row, int column) const {
        return values_[static_cast<std::size_t>(row * 4 + column)];
    }

    bool operator==(const Matrix4& other) const {
        return values_ == other.values_;
    }

    bool operator!=(const Matrix4& other) const {
        return !(*this == other);
    }

    Matrix4 operator+(const Matrix4& other) const {
        Matrix4 result = zero();
        for (int index = 0; index < 16; ++index) {
            result.values_[static_cast<std::size_t>(index)] =
                values_[static_cast<std::size_t>(index)] + other.values_[static_cast<std::size_t>(index)];
        }
        return result;
    }

    Matrix4 operator-(const Matrix4& other) const {
        Matrix4 result = zero();
        for (int index = 0; index < 16; ++index) {
            result.values_[static_cast<std::size_t>(index)] =
                values_[static_cast<std::size_t>(index)] - other.values_[static_cast<std::size_t>(index)];
        }
        return result;
    }

    Matrix4 operator*(float scalar) const {
        Matrix4 result = zero();
        for (int index = 0; index < 16; ++index) {
            result.values_[static_cast<std::size_t>(index)] =
                values_[static_cast<std::size_t>(index)] * scalar;
        }
        return result;
    }

    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result = zero();
        for (int row = 0; row < 4; ++row) {
            for (int column = 0; column < 4; ++column) {
                for (int index = 0; index < 4; ++index) {
                    result(row, column) += (*this)(row, index) * other(index, column);
                }
            }
        }
        return result;
    }

    Point3 operator*(const Point3& point) const {
        float x = (*this)(0, 0) * point.x + (*this)(0, 1) * point.y + (*this)(0, 2) * point.z + (*this)(0, 3);
        float y = (*this)(1, 0) * point.x + (*this)(1, 1) * point.y + (*this)(1, 2) * point.z + (*this)(1, 3);
        float z = (*this)(2, 0) * point.x + (*this)(2, 1) * point.y + (*this)(2, 2) * point.z + (*this)(2, 3);
        float w = (*this)(3, 0) * point.x + (*this)(3, 1) * point.y + (*this)(3, 2) * point.z + (*this)(3, 3);

        if (w != 0.0f && w != 1.0f) {
            x /= w;
            y /= w;
            z /= w;
        }

        return Point3(x, y, z);
    }

    Vec3 operator*(const Vec3& vector) const {
        return Vec3(
            (*this)(0, 0) * vector.x + (*this)(0, 1) * vector.y + (*this)(0, 2) * vector.z,
            (*this)(1, 0) * vector.x + (*this)(1, 1) * vector.y + (*this)(1, 2) * vector.z,
            (*this)(2, 0) * vector.x + (*this)(2, 1) * vector.y + (*this)(2, 2) * vector.z
        );
    }

    Matrix4& operator+=(const Matrix4& other) {
        *this = *this + other;
        return *this;
    }

    Matrix4& operator-=(const Matrix4& other) {
        *this = *this - other;
        return *this;
    }

    Matrix4& operator*=(float scalar) {
        *this = *this * scalar;
        return *this;
    }

    Matrix4& operator*=(const Matrix4& other) {
        *this = *this * other;
        return *this;
    }

    static Matrix4 makeTranslation(const Point3& position) {
        return Matrix4(
            1.0f, 0.0f, 0.0f, position.x,
            0.0f, 1.0f, 0.0f, position.y,
            0.0f, 0.0f, 1.0f, position.z,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    static Matrix4 makeScale(const Vec3& scale) {
        return Matrix4(
            scale.x, 0.0f, 0.0f, 0.0f,
            0.0f, scale.y, 0.0f, 0.0f,
            0.0f, 0.0f, scale.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    static Matrix4 makeRotationX(float angle) {
        float radians = angle * 3.14159265358979323846f / 180.0f;
        float cosine = std::cos(radians);
        float sine = std::sin(radians);

        return Matrix4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosine, -sine, 0.0f,
            0.0f, sine, cosine, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    static Matrix4 makeRotationY(float angle) {
        float radians = angle * 3.14159265358979323846f / 180.0f;
        float cosine = std::cos(radians);
        float sine = std::sin(radians);

        return Matrix4(
            cosine, 0.0f, sine, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sine, 0.0f, cosine, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    static Matrix4 makeRotationZ(float angle) {
        float radians = angle * 3.14159265358979323846f / 180.0f;
        float cosine = std::cos(radians);
        float sine = std::sin(radians);

        return Matrix4(
            cosine, -sine, 0.0f, 0.0f,
            sine, cosine, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

private:
    std::array<float, 16> values_;
};
