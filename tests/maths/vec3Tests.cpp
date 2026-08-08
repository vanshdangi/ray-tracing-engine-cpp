#include <gtest/gtest.h>

#include <maths/vec3.hpp>

TEST(Vec3Test, Addition) {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(4.0f, 5.0f, 6.0f);

    Vec3 result = a + b;

    EXPECT_FLOAT_EQ(result.x, 5.0f);
    EXPECT_FLOAT_EQ(result.y, 7.0f);
    EXPECT_FLOAT_EQ(result.z, 9.0f);
}

TEST(Vec3Test, Subtraction) {
    Vec3 a(4.0f, 5.0f, 6.0f);
    Vec3 b(1.0f, 2.0f, 3.0f);

    Vec3 result = a - b;

    EXPECT_FLOAT_EQ(result.x, 3.0f);
    EXPECT_FLOAT_EQ(result.y, 3.0f);
    EXPECT_FLOAT_EQ(result.z, 3.0f);
}

TEST(Vec3Test, ScalarMultiplication) {
    Vec3 v(1.0f, 2.0f, 3.0f);

    Vec3 result = v * 2.0f;

    EXPECT_FLOAT_EQ(result.x, 2.0f);
    EXPECT_FLOAT_EQ(result.y, 4.0f);
    EXPECT_FLOAT_EQ(result.z, 6.0f);
}

TEST(Vec3Test, Magnitude) {
    Vec3 v(3.0f, 4.0f, 0.0f);

    EXPECT_FLOAT_EQ(v.magnitude(), 5.0f);
    EXPECT_FLOAT_EQ(v.magnitudeSquared(), 25.0f);
}

TEST(Vec3Test, Normalization) {
    Vec3 v(3.0f, 4.0f, 0.0f);

    Vec3 result = v.normalized();

    EXPECT_FLOAT_EQ(result.x, 0.6f);
    EXPECT_FLOAT_EQ(result.y, 0.8f);
    EXPECT_FLOAT_EQ(result.z, 0.0f);
}

TEST(Vec3Test, DotProduct) {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(4.0f, 5.0f, 6.0f);

    EXPECT_FLOAT_EQ(a.dot(b), 32.0f);
}

TEST(Vec3Test, CrossProduct) {
    Vec3 a(1.0f, 0.0f, 0.0f);
    Vec3 b(0.0f, 1.0f, 0.0f);

    Vec3 result = a.cross(b);

    EXPECT_FLOAT_EQ(result.x, 0.0f);
    EXPECT_FLOAT_EQ(result.y, 0.0f);
    EXPECT_FLOAT_EQ(result.z, 1.0f);
}

TEST(Vec3Test, ZeroVectorNormalization) {
    Vec3 v(0.0f, 0.0f, 0.0f);

    Vec3 result = v.normalized();

    EXPECT_FLOAT_EQ(result.x, 0.0f);
    EXPECT_FLOAT_EQ(result.y, 0.0f);
    EXPECT_FLOAT_EQ(result.z, 0.0f);
}