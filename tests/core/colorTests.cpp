#include <gtest/gtest.h>
#include <core/color.hpp>

TEST(ColorTest, Addition) {
    const Color result = Color(0.1f, 0.2f, 0.3f) + Color(0.4f, 0.5f, 0.6f);

    EXPECT_NEAR(result.r, 0.5f, 0.000001f);
    EXPECT_NEAR(result.g, 0.7f, 0.000001f);
    EXPECT_NEAR(result.b, 0.9f, 0.000001f);
}

TEST(ColorTest, Subtraction) {
    EXPECT_EQ(Color(0.8f, 0.7f, 0.6f) - Color(0.1f, 0.2f, 0.3f),
              Color(0.7f, 0.5f, 0.3f));
}

TEST(ColorTest, ScalarOperations) {
    Color color(0.1f, 0.2f, 0.3f);

    EXPECT_EQ(color * 2.0f, Color(0.2f, 0.4f, 0.6f));
    EXPECT_EQ(color / 2.0f, Color(0.05f, 0.1f, 0.15f));
}

TEST(ColorTest, CompoundOperations) {
    Color color(0.1f, 0.2f, 0.3f);

    color += Color(0.4f, 0.5f, 0.6f);
    EXPECT_NEAR(color.r, 0.5f, 0.000001f);
    EXPECT_NEAR(color.g, 0.7f, 0.000001f);
    EXPECT_NEAR(color.b, 0.9f, 0.000001f);

    color -= Color(0.1f, 0.2f, 0.3f);
    EXPECT_EQ(color, Color(0.4f, 0.5f, 0.6f));

    color *= 2.0f;
    EXPECT_EQ(color, Color(0.8f, 1.0f, 1.2f));

    color /= 2.0f;
    EXPECT_EQ(color, Color(0.4f, 0.5f, 0.6f));
}

TEST(ColorTest, FromSrgbConvertsChannelsToLinear) {
    const Color color = Color::fromSrgb(0.5f, 0.04045f, 1.0f);

    EXPECT_NEAR(color.r, 0.214041f, 0.000001f);
    EXPECT_NEAR(color.g, 0.003131f, 0.000001f);
    EXPECT_FLOAT_EQ(color.b, 1.0f);
}