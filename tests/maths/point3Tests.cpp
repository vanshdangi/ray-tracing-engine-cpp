#include <gtest/gtest.h>
#include <maths/point3.hpp>

TEST(Point3Test, Subtraction) {
    Point3 a(4.0f, 5.0f, 6.0f);
    Point3 b(1.0f, 2.0f, 3.0f);

    Vec3 result = a - b;

    EXPECT_EQ(a - b, Vec3(3.0f, 3.0f, 3.0f));
}

TEST(Point3Test, AdditionWithVector) {
    Point3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(4.0f, 5.0f, 6.0f);

    Point3 result = a + b;

    EXPECT_EQ(a + b, Point3(5.0f, 7.0f, 9.0f));
}

TEST(Point3Test, SubtractionWithVector) {
    Point3 a(4.0f, 5.0f, 6.0f);
    Vec3 b(1.0f, 2.0f, 3.0f);

    Point3 result = a - b;

    EXPECT_EQ(a - b, Point3(3.0f, 3.0f, 3.0f));
}

TEST(Point3Test, AdditionAssignmentWithVector) {
    Point3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(4.0f, 5.0f, 6.0f);

    a += b;

    EXPECT_EQ(a, Point3(5.0f, 7.0f, 9.0f));
}

TEST(Point3Test, SubtractionAssignmentWithVector) {
    Point3 a(4.0f, 5.0f, 6.0f);
    Vec3 b(1.0f, 2.0f, 3.0f);

    a -= b;

    EXPECT_EQ(a, Point3(3.0f, 3.0f, 3.0f));

}