#include <gtest/gtest.h>
#include <core/color.hpp>

TEST(ColorTest, Addition) {
    EXPECT_EQ(Color(1.0f, 2.0f, 3.0f) + Color(4.0f, 5.0f, 6.0f),
              Color(5.0f, 7.0f, 9.0f));
}

TEST(ColorTest, Subtraction) {
    EXPECT_EQ(Color(4.0f, 5.0f, 6.0f) - Color(1.0f, 2.0f, 3.0f),
              Color(3.0f, 3.0f, 3.0f));
}

TEST(ColorTest, ScalarOperations) {
    Color color(1.0f, 2.0f, 3.0f);

    EXPECT_EQ(color * 2.0f, Color(2.0f, 4.0f, 6.0f));
    EXPECT_EQ(color / 2.0f, Color(0.5f, 1.0f, 1.5f));
}

TEST(ColorTest, CompoundOperations) {
    Color color(1.0f, 2.0f, 3.0f);

    color += Color(4.0f, 5.0f, 6.0f);
    EXPECT_EQ(color, Color(5.0f, 7.0f, 9.0f));

    color -= Color(1.0f, 2.0f, 3.0f);
    EXPECT_EQ(color, Color(4.0f, 5.0f, 6.0f));

    color *= 2.0f;
    EXPECT_EQ(color, Color(8.0f, 10.0f, 12.0f));

    color /= 2.0f;
    EXPECT_EQ(color, Color(4.0f, 5.0f, 6.0f));
}