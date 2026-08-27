#include <gtest/gtest.h>
#include <core/ray.hpp>

TEST(RayTest, RayEquationTest) {
    Ray r;
    r.origin = Point3(1.0f, 2.0f,3.0f);
    r.direction = Vec3(2.0f ,4.0f ,6.0f);

    EXPECT_EQ(r.at(-1.0f), Point3(-1.0f, -2.0f, -3.0f));
    EXPECT_EQ(r.at(0.0f), Point3(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(r.at(0.5f), Point3(2.0f, 4.0f, 6.0f));
    EXPECT_EQ(r.at(1.0f), Point3(3.0f, 6.0f, 9.0f));
    EXPECT_EQ(r.at(2.0f), Point3(5.0f, 10.0f, 15.0f));
}