#include <gtest/gtest.h>
#include <world/objects/triangle.hpp>

namespace {

Triangle makeTriangle() {
    return Triangle(
        Point3(0.0f, 0.0f, 0.0f),
        Point3(1.0f, 0.0f, 0.0f),
        Point3(0.0f, 1.0f, 0.0f),
        Material()
    );
}

}

TEST(TriangleTest, RayHitsInsideTriangle) {
    Triangle triangle = makeTriangle();
    Ray ray{Point3(0.25f, 0.25f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(triangle.intersects(ray).t, 1.0f);
}

TEST(TriangleTest, RayMissesOutsideTriangle) {
    Triangle triangle = makeTriangle();
    Ray ray{Point3(0.75f, 0.75f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(triangle.intersects(ray).t, -1.0f);
}

TEST(TriangleTest, ParallelRayDoesNotIntersect) {
    Triangle triangle = makeTriangle();
    Ray ray{Point3(0.25f, 0.25f, 1.0f), Vec3(1.0f, 0.0f, 0.0f)};

    EXPECT_FLOAT_EQ(triangle.intersects(ray).t, -1.0f);
}

TEST(TriangleTest, RayPointingAwayDoesNotIntersect) {
    Triangle triangle = makeTriangle();
    Ray ray{Point3(0.25f, 0.25f, 1.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(triangle.intersects(ray).t, -1.0f);
}

TEST(TriangleTest, NormalIsConstantAcrossTriangle) {
    Triangle triangle = makeTriangle();

    EXPECT_EQ(triangle.getNormal(Point3(0.25f, 0.25f, 0.0f)), Vec3(0.0f, 0.0f, -1.0f));
    EXPECT_EQ(triangle.getNormal(Point3(0.0f, 0.0f, 0.0f)), Vec3(0.0f, 0.0f, -1.0f));
}
